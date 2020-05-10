#version 330 core

out vec4 out_color;

uniform vec2	iResolution;
uniform float	iTime;


#define PI		3.14159265
#define EPS	0.001
#define MAX_STEPS 200
#define MAX_DIST 100.0
#define BOARD_SQUARE 1.0
#define SPHERE1 0
#define SPHERE2 1
#define SPHERE3 2
#define HEART	3
#define PLANE	4

struct Intersect {
    int		idx;
    float	dist;};

struct Point	{
	vec3	pos;
    vec3	col;};
    
vec3 diffCol[] = vec3[](vec3(0.0, 0.0, 1.0),
						vec3(1.0, 0.0, 1.0),
						vec3(0.0, 1.0, 0.3),
						vec3(1.0, 0.1, 0.1));

float diff[] =  float[](0.4, 0.5,  0.4,  0.8,  0.7);
float spec[] =  float[](0.9, 1.2,  1.1,  1.9,  0.3);
float specPow[]=float[](8.0, 16.0, 16.0, 32.0, 4.0);

vec3 lightPos = vec3(-3.0, 3.0, 5.0);

float sphereDist(in vec3 pos, in float radius, in vec3 center)
{
    return (length(pos + center) - radius);
}

float planeDist(in vec3 pos, in vec4 normal)
{
    return (dot(pos, normal.xyz) + normal.w);
}

float heartDist(vec3 q, float r)
{
    float ani = pow(0.5 + 0.5 * sin(7.0 * iTime + q.y / 20.0), 4.0);
    q *= 1.0 - 0.2 * vec3(1.0,0.5,1.0)*ani;
    q.y -= 0.2 * ani;
    float x = abs(q.x);
        
    float y = q.y;
    float z = q.z;
    y = -0.5 + y * 1.2 - x * sqrt(max((5.0 - x) / 10.0, 0.0));
    float res = sqrt(x*x + y*y + z*z) - r;
    return res;
}

Intersect sceneDist(vec3 pos)
{
	float SINt = sin(iTime);
    float SINt03 = sin(iTime * 0.3);
    float COSt03 = cos(iTime * 0.3);
    float sphereD1_1 = sphereDist(pos, 1.8, vec3(0, 0.1, 0));
	float sphereD1_2 = sphereDist(pos, 1.8, vec3(0, 0.1 - 2.0*(0.5 + 0.5 * SINt), 0));
    float sphereD2 = sphereDist(pos, 0.7, vec3(2.8 * COSt03, SINt, 2.8 * SINt03));
    float sphereD3 = sphereDist(pos, 0.5, vec3(3.8 * SINt03, cos(iTime), 3.8 * COSt03));
    float planeD = planeDist(pos, vec4(0.0, 1.0, 0.0, 2.2));
    float heartD = heartDist(pos, 0.5);
    Intersect res;
    
    // hollow sphere
	sphereD1_1 = abs(sphereD1_1) - 0.03;
	sphereD1_1 = max(sphereD1_1, pos.y);
	sphereD1_2 = abs(sphereD1_2) - 0.03;
	sphereD1_2 = max(sphereD1_2, - (pos.y - 2.0 * (0.5 + 0.5 * SINt)));

    res.dist = min(min(min(min(min(sphereD1_1, sphereD1_2), sphereD2), sphereD3), planeD), heartD);
    if (res.dist == sphereD1_1 || res.dist == sphereD1_2)
        res.idx = SPHERE1;
    else if (res.dist == sphereD2)
		res.idx = SPHERE2;
    else if (res.dist == sphereD3)
		res.idx = SPHERE3;
    else if (res.dist == heartD)
        res.idx = HEART;     
    else
        res.idx = PLANE;
    return (res);
}

vec3 getNormal(vec3 p)
{
    return normalize(vec3(
    sceneDist(vec3(p.x + EPS, p.y, p.z)).dist - sceneDist(vec3(p.x - EPS, p.y, p.z)).dist,
    sceneDist(vec3(p.x, p.y + EPS, p.z)).dist - sceneDist(vec3(p.x, p.y - EPS, p.z)).dist,
    sceneDist(vec3(p.x, p.y, p.z  + EPS)).dist - sceneDist(vec3(p.x, p.y, p.z - EPS)).dist));
}

Intersect rayMarch(vec3 ro, vec3 rd)
{
    Intersect d;
    Intersect intersect;
    for (int i = 0; i < MAX_STEPS; ++i)
    {
        d = sceneDist(ro + intersect.dist * rd);
        intersect.dist += d.dist;
        if (intersect.dist < EPS || intersect.dist >= MAX_DIST)
            break;
    }
    intersect.idx = d.idx;
    return (intersect);
}

float getShadow(in vec3 ro, in vec3 rd, in float min_t, in float max_t, in float k)
{
    float res = 1.0;
    for (float t = min_t; t < max_t;)
    {
        float dist = sceneDist(ro + rd * t).dist;
        if (dist < EPS)
            return 0.0;
        res = min(res, k * dist / t);
        t += dist;
    }
    return (res);
}

vec3 boardColor(vec3 p)
{
    vec3 pos = vec3(p.x, p.y, p.z - (0.7 * iTime));
    
    if ((mod(pos.x,(BOARD_SQUARE * 2.0)) < BOARD_SQUARE && mod(pos.z,(BOARD_SQUARE * 2.0)) > BOARD_SQUARE) ||
        (mod(pos.x,(BOARD_SQUARE * 2.0)) > BOARD_SQUARE && mod(pos.z,(BOARD_SQUARE * 2.0)) < BOARD_SQUARE))
		return vec3(1.0);
    else
		return vec3(0.0);
}

vec3 getLight(int idx, vec3 pos, vec3 eye)
{
    vec3 diffuseCol;
    if (idx == PLANE)
        diffuseCol = boardColor(pos);
    else
        diffuseCol = diffCol[idx];
    vec3 specularCol = vec3(1.0);
    vec3 Norm = getNormal(pos);
    vec3 Ldir = normalize(lightPos - pos);
    vec3 View = normalize(eye - pos);
    vec3 Refl = normalize(reflect(-Ldir, Norm));

    float dotLN = dot(Ldir, Norm);
    float dotRV = dot(Refl, View);

    // light not visible
    if (dotLN < 0.0)
        return vec3(0.0);

    // light reflection in opposite direction, apply only diffuse lighting
    if (dotRV < 0.0)
        return 1.7 * (diffuseCol * dotLN) * diff[idx];

    vec3 half_direction = normalize(normalize(Ldir) + View);
    float specular = pow(max(dot(half_direction, Norm), 0.0), specPow[idx]);
    return 1.7 * diffuseCol * dotLN * diff[idx] + specularCol * specular * spec[idx];
}

Point getColor(vec3 ro, vec3 rd, vec3 eye)
{
    Point point;
    Intersect intersect = rayMarch(ro, rd);
    point.pos = ro + intersect.dist * rd;
    if (intersect.dist > MAX_DIST - EPS)
        point.col = vec3(0.0, 0.0, 1.0 - 2.0 * rd.y); //background
    else
    {    
		//ambient
        vec3 ambientCol = vec3(0.2, 0.4, 0.7);    
    	vec3 ambientLight = 0.25 * vec3(1.0);
    	point.col = ambientLight * ambientCol;

    	//shadow
    	vec3 shadowDir = normalize(lightPos - point.pos);
    	float shadow = getShadow(point.pos + shadowDir, shadowDir, 0.0, MAX_DIST, 8.0);

    	//diffuse + specular
    	vec3 blinn_phong = getLight(intersect.idx, point.pos, eye) * shadow;
    	point.col += blinn_phong;
    }
    return point;
}


void main()
{
    vec2 aspectRatio = vec2(iResolution.x / iResolution.y, 1.0);
    vec2 uv = aspectRatio * (gl_FragCoord.xy / iResolution.xy/2.0 - 0.5);
//    vec2 mouse = 4.0 * (iMouse.xy / iResolution.xy);
    vec3 rd = normalize(vec3(uv, -1.0));
    vec3 ro = vec3(0.0, 0.0, 8.0);

    Point point = getColor(ro, rd, ro);
	vec3 color = point.col;   
    
    vec3 rd1 = rd - (2.0 * dot(rd, getNormal(point.pos)) * getNormal(point.pos));
    Point point2 = getColor(point.pos + rd1, rd1, ro);
    color = mix(color, point2.col, 0.22);
    
    vec3 rd2 = rd1 - (2.0 * dot(rd1, getNormal(point2.pos)) * getNormal(point2.pos));
    Point point3 = getColor(point2.pos + rd2, rd2, ro);
    color = mix(color, point3.col, 0.12);
    
    out_color = vec4(color, 1.0);
}
