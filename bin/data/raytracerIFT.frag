#version 120

const float M_PI = 3.14159265358979323846;
const float INFINITY = 1000000000.;
const int PLANE = 1;
const int SPHERE_0 = 2;
const int SPHERE_1 = 3;
const int SPHERE_2 = 4;
const int CUBE = 5;

uniform float u_aspect_ratio;
varying vec2 v_position;

uniform vec3 sphere_position_0;
uniform float sphere_radius_0;
uniform vec3 sphere_color_0;

uniform vec3 sphere_position_1;
uniform float sphere_radius_1;
uniform vec3 sphere_color_1;

uniform vec3 sphere_position_2;
uniform float sphere_radius_2;
uniform vec3 sphere_color_2;

uniform vec3 cube_position;
uniform vec3 cube_color;
uniform vec3 cube_max;
uniform vec3 cube_min;

uniform vec3 plane_position;
uniform vec3 plane_normal;

uniform float light_intensity;
uniform vec2 light_specular;
uniform vec3 light_position;
uniform vec3 light_color;

uniform float ambient;
uniform vec3 O;
/*
vec2 intersect_Box(vec3 entryPoint , vec3 rayDir, vec3 boxmin, vec3 boxmax)//, float *tnear, float *tfar)
{
    // compute intersection of ray with all six bbox planes
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
range intersectBox(vec3 entryPoint , vec3 rayDir, vec3 boxmin, vec3 boxmax)//, float *tnear, float *tfar)
{
    // compute intersection of ray with all six bbox planes
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
}



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
    return INFINITY;
}

float intersect_plane(vec3 O, vec3 D, vec3 P, vec3 N) {
    float denom = dot(D, N);
    if (abs(denom) < 1e-6) {
        return INFINITY;
    }
    float d = dot(P - O, N) / denom;
    if (d < 0.) {
        return INFINITY;
    }
    return d;
}

vec3 run(float x, float y) {
    vec3 Q = vec3(x, y, 0.);
    vec3 D = normalize(Q - O);
    int depth = 0;
    range t31;
    float t_plane, t0, t1, t2, t3n, t3f;
    t3n = t31.tNear; 
    t3f = t31.tFar;
    vec3 rayO = O;
    vec3 rayD = D;
    vec3 col = vec3(0.1255, 0.0706, 0.3255);
    vec3 col_ray;
    float reflection = 1.;

    int object_index;
    vec3 object_color;
    vec3 object_normal;
    float object_reflection;
    vec3 M;
    vec3 N, toL, toO;

    while (depth < 5) {

        t_plane = intersect_plane(rayO, rayD, plane_position, plane_normal);
        t0 = intersect_sphere(rayO, rayD, sphere_position_0, sphere_radius_0);
        t1 = intersect_sphere(rayO, rayD, sphere_position_1, sphere_radius_1);
        t2 = intersect_sphere(rayO, rayD, sphere_position_2, sphere_radius_2);
        //t31 = intersect_box(ray0, rayD, cube_min, cube_max);
        if (t_plane < min(t0, t1)) {
            M = rayO + rayD * t_plane;
            object_normal = plane_normal;

	    //if (mod(int(2*M.x), 2) == mod(int(2*M.z), 2)) { /* rect plane */
            object_color = vec3(0., 0.546, 1.);
            //} else {
            //	object_color = vec3(1., 0., 0.);
            //}

            object_reflection = .35;
            object_index = PLANE;
        }
        else if (t0 < t1 && t0 < t2 ) {
            // Sphere 0.
            M = rayO + rayD * t0;
            object_normal = normalize(M - sphere_position_0);
            object_color = sphere_color_0;
            object_reflection = .5;
            object_index = SPHERE_0;
        }
        else if (t1 < t0 && t1 < t2 ) {
            // Sphere 1.
            M = rayO + rayD * t1;
            object_normal = normalize(M - sphere_position_1);
            object_color = sphere_color_1;
            object_reflection = .5;
            object_index = SPHERE_1;
        }
        else if (t2 < t0 && t2 < t1 ) {
            // Sphere 2.
            M = rayO + rayD * t2;
            object_normal = normalize(M - sphere_position_2);
            object_color = sphere_color_2;
            object_reflection = .5;
            object_index = SPHERE_2;
        }/*
        else if(t31n<t31f)
            {
                M = rayO + rayD * t31n;
                object_normal = normalize(M - cube_position);
                object_color = cube_color;
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

        /* shadow non bellaM = rayO + rayD * t1; */
        if (object_index == PLANE) {

            t0 = intersect_sphere(M + N * .0001, toL, sphere_position_0, sphere_radius_0);
            t1 = intersect_sphere(M + N * .0001, toL, sphere_position_1, sphere_radius_1);
            t2 = intersect_sphere(M + N * .0001, toL, sphere_position_2, sphere_radius_2);
            //t31 = intersect_cube(M + N * .0001, toL, cube_min, cube_max);
            
            if (min(t0, t1) < INFINITY) {
                break;
            }

        }

        col_ray = vec3(ambient, ambient, ambient);
        col_ray += light_intensity * max(dot(N, toL), 0.) * object_color;
        col_ray += light_specular.x * light_color * pow(max(dot(N, normalize(toL + toO)), 0.), light_specular.y);

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
    gl_FragColor = vec4(run(pos.x*u_aspect_ratio, pos.y), 1.);
}