#include <stdint.h>
#include <stdlib.h>
#include <rocm_smi/rocm_smi.h>

#ifndef STRUCTS_H
#define STRUCTS_H
#include "../structs.h"
#endif

#ifndef INIT_H
#define INIT_H
#include "./init.h"
#endif

result_uint32_t busy_percent(uint32_t dv_ind)
{
    if (init.status != RSMI_STATUS_SUCCESS)
    {
        result_uint32_t error = {init.status, 0};
        return error;
    }

    uint32_t percent;
    rsmi_status_t ret = rsmi_dev_busy_percent_get(dv_ind, &percent);

    result_uint32_t res = {ret, percent};
    return res;
}

typedef struct result_util_counter
{
    uint16_t status;
    uint64_t gfx;
    uint64_t mem;
} result_util_counter;

result_util_counter util_counters(uint32_t dv_ind)
{
    if (init.status != RSMI_STATUS_SUCCESS)
    {
        result_util_counter error = {init.status, 0, 0};
        return error;
    }
    uint32_t count = 2;
    uint64_t timestamp;
    rsmi_utilization_counter_t *counters = (rsmi_utilization_counter_t *)malloc(sizeof(rsmi_utilization_counter_t) * count);
    counters[0].type = RSMI_UTILIZATION_COUNTER_FIRST;
    counters[1].type = RSMI_COARSE_GRAIN_MEM_ACTIVITY;

    rsmi_status_t ret = rsmi_utilization_count_get(dv_ind, counters, count, &timestamp);

    result_util_counter res = {ret, counters[0].value, counters[1].value};
    return res;
}

result_uint32_t perf_level(uint32_t dv_ind)
{
    if (init.status != RSMI_STATUS_SUCCESS)
    {
        result_uint32_t error = {init.status, 0};
        return error;
    }

    rsmi_dev_perf_level_t level;
    rsmi_status_t ret = rsmi_dev_perf_level_get(dv_ind, &level);

    result_uint32_t res = {ret, level};
    return res;
}

typedef struct result_overdrive_levels
{
    uint16_t status;
    uint32_t graphics;
    uint32_t memory;
} result_overdrive_levels;

result_overdrive_levels overdrive_levels(uint32_t dv_ind)
{
    rsmi_status_t ret = init.status;
    result_overdrive_levels res = {ret, 0, 0};
    if (ret != RSMI_STATUS_SUCCESS)
        return res;

    uint32_t graphics, memory;

    ret = rsmi_dev_overdrive_level_get(dv_ind, &graphics);
    if (ret != RSMI_STATUS_SUCCESS)
        return res;

    res.graphics = graphics;

    ret = rsmi_dev_mem_overdrive_level_get(dv_ind, &memory);
    if (ret != RSMI_STATUS_SUCCESS)
        return res;

    res.memory = memory;
    return res;
}

typedef struct result_frequencies
{
    uint16_t status;
    uint32_t num_supported;
    uint32_t current;
    uint64_t *frequencies;
} result_frequencies;

result_frequencies frequency(uint32_t dv_ind, uint32_t clk_type)
{
    if (init.status != RSMI_STATUS_SUCCESS)
    {
        result_frequencies error = {init.status, 0, 0, NULL};
        return error;
    }

    rsmi_frequencies_t freq = {0, 0, {0}};
    rsmi_status_t ret = rsmi_dev_gpu_clk_freq_get(dv_ind, clk_type, &freq);

    uint64_t *ptr = (uint64_t *)malloc(sizeof(uint64_t) * freq.num_supported);
    for (size_t i = 0; i < freq.num_supported; i++)
        ptr[i] = freq.frequency[i];

    result_frequencies res = {ret, freq.num_supported, freq.current, ptr};
    return res;
}

typedef struct result_volt_curve
{
    uint16_t status;
    uint32_t num_regions;
    uint64_t curr_sclk_range_min;
    uint64_t curr_sclk_range_max;
    uint64_t sclk_limit_min;
    uint64_t sclk_limit_max;
    uint64_t curr_mclk_range_min;
    uint64_t curr_mclk_range_max;
    uint64_t mclk_limit_min;
    uint64_t mclk_limit_max;
    rsmi_od_vddc_point_t *points;

} result_volt_curve;

result_volt_curve volt_curve(uint32_t dv_ind)
{
    if (init.status != RSMI_STATUS_SUCCESS)
    {
        result_volt_curve error = {
            .status = init.status,
            .num_regions = 0,
            .curr_sclk_range_min = 0,
            .curr_sclk_range_max = 0,
            .sclk_limit_min = 0,
            .sclk_limit_max = 0,
            .curr_mclk_range_min = 0,
            .curr_mclk_range_max = 0,
            .mclk_limit_min = 0,
            .mclk_limit_max = 0,
            .points = NULL
        };
        return error;
    }

    rsmi_od_volt_freq_data_t volt_c;
    rsmi_status_t ret = rsmi_dev_od_volt_info_get(dv_ind, &volt_c);

    uint32_t num_regions = volt_c.num_regions;
    rsmi_od_vddc_point_t *points = (rsmi_od_vddc_point_t *)malloc(sizeof(rsmi_od_vddc_point_t) * num_regions);
    for (size_t i = 0; i < num_regions; i++)
        points[i] = volt_c.curve.vc_points[i];
    
    result_volt_curve res = {
        .status = ret,
        .num_regions = 0,
        .curr_sclk_range_min = 0,
        .curr_sclk_range_max = 0,
        .sclk_limit_min = 0,
        .sclk_limit_max = 0,
        .curr_mclk_range_min = 0,
        .curr_mclk_range_max = 0,
        .mclk_limit_min = 0,
        .mclk_limit_max = 0,
        .points = points
    };
    return res;
}