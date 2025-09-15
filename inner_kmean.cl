__kernel void example_kernel(__global double *image, __global double *end_distance,  __global double *result, int size, double kernel_bandwidth)
{
    int gid = get_global_id(0);
    double total_weight = 0;
    double p_new_r = 0;
    double p_new_g = 0;
    double p_new_b = 0;
    double p_new_x = 0;
    double p_new_y = 0;

    for (int j = 0; j < size; j++)
    {
        // Calculate distance
        double distance_r = (image[5*gid+2] - image[5*j+2]) / kernel_bandwidth;
        double distance_g = (image[5*gid+3] - image[5*j+3]) / kernel_bandwidth;
        double distance_b = (image[5*gid+4] - image[5*j+4]) / kernel_bandwidth;
        double distance_x = (image[5*gid] - image[5*j]) / kernel_bandwidth;
        double distance_y = (image[5*gid+1] - image[5*j+1]) / kernel_bandwidth;
        double distance = sqrt(distance_r * distance_r + distance_g * distance_g + distance_b * distance_b + distance_x * distance_x + distance_y * distance_y);

        // Calculate weight
        double weight = exp(-pow(distance, 2) / 2) / sqrt(2 * M_PI);

        // Update new pixel values
        p_new_r += image[5*j+2] * weight;
        p_new_g += image[5*j+3] * weight;
        p_new_b += image[5*j+4] * weight;
        p_new_x += image[5*j] * weight;
        p_new_y += image[5*j+1] * weight;

        total_weight += weight;
    }

    // Normalize new pixel values
    p_new_r /= total_weight;
    p_new_g /= total_weight;
    p_new_b /= total_weight;
    p_new_x /= total_weight;
    p_new_y /= total_weight;

    // Calculate end_distance
    double end_distance_r = (p_new_r - image[5*gid+2]);
    double end_distance_g = (p_new_g - image[5*gid+3]);
    double end_distance_b = (p_new_b - image[5*gid+4]);
    double distance_x = (image[5*gid] - image[5*j]);
    double distance_y = (image[5*gid+1] - image[5*j+1]);
    end_distance[gid] = sqrt(end_distance_r * end_distance_r + end_distance_g * end_distance_g + end_distance_b * end_distance_b + distance_x * distance_x + distance_y * distance_y);

    // Update image buffer
    result[5*gid] = p_new_x;
    result[5*gid+1] = p_new_y;
    result[5*gid+2] = p_new_r;
    result[5*gid+3] = p_new_g;
    result[5*gid+4] = p_new_b;
}