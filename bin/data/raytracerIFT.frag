#version 120

const float M_PI = 3.14159265358979323846;
const float NFTY = 1000000000.;
const int PLAN = 1;
const int SPHERE = 2;
const int SPHERE2 = 3;
const int SPHERE3 = 4;
const int CUBE = 5;

uniform float u_aspect_ratio;
varying vec2 v_position;

uniform vec3 sphere_position_0;
uniform float sphere_rayon_0;
uniform vec3 sphere_couleur_0;

uniform vec3 sphere_position_1;
uniform float sphere_rayon_1;
uniform vec3 sphere_couleur_1;

uniform vec3 sphere_position_2;
uniform float sphere_rayon_2;
uniform vec3 sphere_couleur_2;

uniform vec3 cube_position;
uniform vec3 cube_couleur;
uniform vec3 cube_max;
uniform vec3 cube_min;

uniform vec3 plan_position;
uniform vec3 plan_normal;

uniform float light_intensity;
uniform vec2 light_specular;
uniform vec3 light_position;
uniform vec3 light_couleur;

uniform float ambient;
uniform vec3 O;
/*
vec2 intersect_Box(vec3 entryPoint , vec3 rayDir, vec3 boxmin, vec3 boxmax)//, float *tnear, float *tfar)
{
    // compute intersection of ray with all six bbox plans
    vec3 invR =  vec3(1.0) / rayDir;
    vec3 tbot = invR * (boxmin - entryPoint);
    vec3 ttop = invR * (boxmax - entryPoint);

    // re-order intersections to find smallest and largest on each axis
    vec3 tmin = min(ttop, tbot);
    vec3 tmax = max(ttop, tbot);

    // find the largest tmin and the smallest tmax
    float largest_tmin = max(max(tmin.x, tmin.y), tmin.z);
    float smallest_tmax = min(min(tmax.x, tmax.y), tmax.z);

	vec2 tMinMax; //= min(largest_tmin, smallest_tmax);
	tMinMax.x = largest_tmin;//near
	tMinMax.y = smallest_tmax;//far
	return tMinMax;
}*/

struct range
{
	float tNear;
	float tFar;
};
/*
range intersectBox(vec3 entryPoint , vec3 rayDir, vec3 boxmin, vec3 boxmax)//, float *tnear, float *tfar)
{
    // compute intersection of ray with all six bbox plans
    vec3 invR;
    if(rayDir.x != 0 && rayDir.y != 0 &&  rayDir.z != 0){
        invR.x = 1.0 / rayDir.x;
        invR.y = 1.0 / rayDir.y;
        invR.z = 1.0 / rayDir.z;
    }
    vec3 tbot = invR * (boxmin - entryPoint);
    vec3 ttop = invR * (boxmax - entryPoint);

    // re-order intersections to find smallest and largest on each axis
    vec3 tmin = min(ttop, tbot);
    vec3 tmax = max(ttop, tbot);

    // find the largest tmin and the smallest tmax
    float largest_tmin = max(max(tmin.x, tmin.y), tmin.z);
    float smallest_tmax = min(min(tmax.x, tmax.y), tmax.z);

	range tMinMax;
	tMinMax.tNear = largest_tmin;
	tMinMax.tFar = smallest_tmax;
	return tMinMax;
}*/



float intersect_sphere(vec3 O, vec3 D, vec3 S, float R) {
    float a = dot(D, D);
    vec3 OS = O - S;
    float b = 2. * dot(D, OS);
    float c = dot(OS, OS) - R * R;
    float disc = b * b - 4. * a * c;
    if (disc > 0.) {
        float distSqrt = sqrt(disc);
        float q = (-b - distSqrt) / 2.0;
        if (b >= 0.) {
            q = (-b + distSqrt) / 2.0;
        }
        float t0 = q / a;
        float t1 = c / q;
        t0 = min(t0, t1);
        t1 = max(t0, t1);
        if (t1 >= 0.) {
            if (t0 < 0.) {
                return t1;
            }
            else {
                return t0;
            }
        }
    }
    return NFTY;
}

float intersect_plan(vec3 O, vec3 D, vec3 P, vec3 N) {
    float denom = dot(D, N);
    if (abs(denom) < 1e-6) {
        return NFTY;
    }
    float d = dot(P - O, N) / denom;
    if (d < 0.) {
        return NFTY;
    }
    return d;
}

vec3 run(float x, float y) {
    vec3 Q = vec3(x, y, 0.);
    vec3 D = normalize(Q - O);
    int depth = 0;
    range t31;
    float t_plan, t0, t1, t2, t3n, t3f;
    t3n = t31.tNear; 
    t3f = t31.tFar;
    vec3 rayO = O;
    vec3 rayD = D;
    vec3 col = vec3(0.1255, 0.0706, 0.3255);
    vec3 col_ray;
    float reflection = 1.;

    int object_index;
    vec3 object_couleur;
    vec3 object_normal;
    float object_reflection;
    vec3 M;
    vec3 N, toL, toO;

    while (depth < 5) {

        t_plan = intersect_plan(rayO, rayD, plan_position, plan_normal);
        t0 = intersect_sphere(rayO, rayD, sphere_position_0, sphere_rayon_0);
        t1 = intersect_sphere(rayO, rayD, sphere_position_1, sphere_rayon_1);
        t2 = intersect_sphere(rayO, rayD, sphere_position_2, sphere_rayon_2);
        //t31 = intersect_box(ray0, rayD, cube_min, cube_max);
        if (t_plan < min(t0, t1)) {
            M = rayO + rayD * t_plan;
            object_normal = plan_normal;
            object_couleur = vec3(0., 0.546, 1.);
            object_reflection = .35;
            object_index = plan;
        }
        else if (t0 < t1 && t0 < t2 ) {
            // Sphere 0.
            M = rayO + rayD * t0;
            object_normal = normalize(M - sphere_position_0);
            object_couleur = sphere_couleur_0;
            object_reflection = .5;
            object_index = SPHERE;
        }
        else if (t1 < t0 && t1 < t2 ) {
            M = rayO + rayD * t1;
            object_normal = normalize(M - sphere_position_1);
            object_couleur = sphere_couleur_1;
            object_reflection = .5;
            object_index = SPHERE2;
        }
        else if (t2 < t0 && t2 < t1 ) {
            M = rayO + rayD * t2;
            object_normal = normalize(M - sphere_position_2);
            object_couleur = sphere_couleur_2;
            object_reflection = .5;
            object_index = SPHERE3;
        }
        /*else if(t31n<t31f)
            {
                M = rayO + rayD * t31n;
                object_normal = normalize(M - cube_position);
                object_couleur = cube_couleur;
                object_reflection = .5;
                object_index = CUBE;
            }*/
        //}

        else {
            break;
        }

        N = object_normal;
        toL = normalize(light_position - M);
        toO = normalize(O - M);
        if (object_index == PLAN) {
            t0 = intersect_sphere(M + N * .0001, toL, sphere_position_0, sphere_rayon_0);
            t1 = intersect_sphere(M + N * .0001, toL, sphere_position_1, sphere_rayon_1);
            t2 = intersect_sphere(M + N * .0001, toL, sphere_position_2, sphere_rayon_2);
            //t31 = intersect_cube(M + N * .0001, toL, cube_min, cube_max);           
            if (min(t0, t1) < NFTY) {
                break;
            }
        }
        col_ray = vec3(ambient, ambient, ambient);
        col_ray += light_intensity * max(dot(N, toL), 0.) * object_couleur;
        col_ray += light_specular.x * light_couleur * pow(max(dot(N, normalize(toL + toO)), 0.), light_specular.y);

        /* end trace_ray */

        rayO = M + N * .0001;
        rayD = normalize(rayD - 2. * dot(rayD, N) * N);
        col += reflection * col_ray;
        reflection *= object_reflection;

        depth++;
    }
    return clamp(col, 0., 1.);
}
void main() {
    vec2 pos = v_position;
    gl_Fragcouleur = vec4(run(pos.x*u_aspect_ratio, pos.y), 1.);
}
