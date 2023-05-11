#define int_RED     4278190335      // 11111111 00000000 00000000 11111111
#define int_GREEN   4278255360      // 11111111 00000000 11111111 00000000    
#define int_BLUE    4294901760      // 11111111 11111111 00000000 00000000

#define int_WHITE   4294967295      // 11111111 11111111 11111111 11111111 
#define int_BLACK   4278190080      // 11111111 00000000 00000000 00000000

#define BOUNCES 2

struct Triangle
{
    float3 v0;
    float3 v1;
    float3 v2;
} ;


// UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-UTILS-

inline float vec_len(float3 vec) 
{
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z); 
}
float3 reflect(float3 ray_direction, float3 normal)
{
    normal = normalize(normal); 
    return ray_direction - 2 * dot(ray_direction, normal) * normal; 
}
float max_val(float val1, float val2)
{
    if (val1 > val2)
    {
        return val1;
    } 
    else
    {
        return val2;
    }
        
}


// COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-COLOR-

uint from_RGBA_to_binary(float3 color)
{
    uchar r = (uchar)(clamp(color.x, .0f, 255.0f));
    uchar g = (uchar)(clamp(color.y, .0f, 255.0f));
    uchar b = (uchar)(clamp(color.z, .0f, 255.0f));
    uchar a = (uchar)(255.0f);
    return (a << 24) | (b << 16) | (g << 8) | r;
}
float4 from_binary_to_RGBA(uint color)
{
    float4 value;
	uint mask = 255;        // 00000000 00000000 00000000 11111111
	value.x = color & mask;
	mask = mask << 8;		// 00000000 00000000 11111111 00000000 
	value.y = (color & mask) >> 8;
	mask = mask << 8;		// 00000000 11111111 00000000 00000000
	value.z = (color & mask) >> 16;
	mask = mask << 8;		// 11111111 00000000 00000000 00000000
	value.w = (color & mask) >> 24;
    return value;
}
float3 from_binary_to_RGB(uint color)
{
    float4 temp = from_binary_to_RGBA(color);
    float3 val = {temp.x, temp.y, temp.z};
    return val;
}

// TRACE-RAY-TRACE-RAY-TRACE-RAY-TRACE-RAY-TRACE-RAY-TRACE-RAY-TRACE-RAY-TRACE-RAY-TRACE-RAY-TRACE-RAY-TRACE-RAY-TRACE-RAY-TRACE-RAY-TRACE-RAY-TRACE-RAY-\n

bool TraceRay(
    __global struct Triangle* triangles,
    uint    triangles_size,
    float3  ray_direction, 
    float3  ray_origin, 
    float*  hit_distance, 
    long*   traingle_id
)
{
    bool has_hit = 0;
    float u, v;
    *hit_distance = FLT_MAX;
    for (size_t triangles_iter = 0; triangles_iter < triangles_size; ++triangles_iter)
    {
        float3 vov1 = triangles[triangles_iter].v1 - triangles[triangles_iter].v0;
        float3 vov2 = triangles[triangles_iter].v2 - triangles[triangles_iter].v0;
        float3 pvec = cross(ray_direction, vov2);
        float  det  = dot(vov1, pvec);

        if (fabs(det) == 0) continue;
        float invertedDet = 1.0f / det;

        float3 tvec = ray_origin - triangles[triangles_iter].v0;
        u = dot(tvec, pvec) * invertedDet;
        if ((u < .0f) || (u > 1.0f))
        {
            continue;
        }
        float3 qvec = cross(tvec, vov1);
        v = dot(ray_direction, qvec) * invertedDet;
        if ((v < .0f) || ((u + v) > 1.0f))
        {
            continue;
        }
        float t = dot(vov2, qvec) * invertedDet;
        if (t < *hit_distance && t > 0)
        {
            *hit_distance = t;
            *traingle_id = triangles_iter;
            has_hit = 1;
        }
    }
    return has_hit;
}


// KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-KERNEL-

__kernel void R1(__global struct Triangle* triangles, __global uint* triangles_size, __global float3* ray_directions, __global float3* ray_origin, __global uint* color_output, __global float* debug_entity){
    
    uint global_id                   =  get_global_id(0);

    float3  color_accumulation       =  {.0f, .0f, .0f};
    float3  pointlight               =  {2.0f, 3.0f, 4.0f};
    float   pointlight_intensity     =  1.0f;
    float   reflectivity             =  0.3f;

    color_output[global_id]          =  int_BLACK; 
    
    float3  current_ray_direction    =   ray_directions[global_id];
    float3  current_ray_origin       =  *ray_origin;

    for (int bounce_iter = 0; bounce_iter < BOUNCES; ++bounce_iter)
    {
        float   hit_distance        =   FLT_MAX;
        long    triangle_id         =   -1;
        float3  intersection_color  =   {.0f,.0f,.0f};
        
        if (TraceRay(triangles, *triangles_size, current_ray_direction, current_ray_origin, &hit_distance, &triangle_id))
        {
            float3  hit_point        = current_ray_origin + current_ray_direction * hit_distance;
            float3  face_normal      = cross(triangles[triangle_id].v1 - triangles[triangle_id].v0,
                                             triangles[triangle_id].v2 - triangles[triangle_id].v0);

            float3  light_direction  = hit_point - pointlight;
            float   cosangle         = max_val(dot(-1*light_direction, face_normal) / (vec_len(face_normal) * vec_len(light_direction)),
                                               0.0f); 

            // если трегуольник повёрнут лицевой стороной:
            if (dot(current_ray_direction, face_normal) <= 0)
            {
                intersection_color = from_binary_to_RGB(int_WHITE);

                float3  shadowRay_origin        = hit_point + face_normal * 0.0001f;
                float3  shadowRay_direction     = pointlight - shadowRay_origin;
                float   shadowRay_hit_distance  = FLT_MAX; 
                long    shadowTriangle_id       = -1;
                
                // Трассируем теневой луч...
                // Если теневой луч встретил пересечение на пути к источнику света:
                if (TraceRay(triangles, *triangles_size, shadowRay_direction, shadowRay_origin, &shadowRay_hit_distance, &shadowTriangle_id)
                    && shadowRay_hit_distance < 1.0f && shadowRay_hit_distance > 0.00005f)
                {
                    intersection_color = from_binary_to_RGB(int_BLACK); // затеняем пиксель
                }        
                else
                {   // Если объект не закрывает тень, то считаем освещение
                    intersection_color *= cosangle * pointlight_intensity;
                    // Имитируем потерю энергии света при каждом отражении
                    if (bounce_iter > 0)
                    {
                        intersection_color *= reflectivity * bounce_iter;
                    }
                }            

                color_accumulation += intersection_color;
                color_output[global_id] = from_RGBA_to_binary(color_accumulation);
            }

            current_ray_direction = reflect(current_ray_direction, face_normal);
            current_ray_origin = hit_point + face_normal* 0.00005f;

        } else {
            break;
        }
    }
}

// ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-ЗАМЕТКИ-
    
    //reflect выдаёт те же результаты, что и glm::reflect
    //normalize выдаёт те же результаты, что и glm::normalize