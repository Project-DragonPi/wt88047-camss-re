/* SPDX-License-Identifier: GPL-2.0-only
 * 
 * Copyright (C) 2022 Project DragonPi
 * Qualcomm Camera Driver Generator (QCDG)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "sensor_lib.h"

#define PRINT_START(x) printf("********* Begin of %s *********\n",x)
#define PRINT_END(x) printf("********* End of %s *********\n",x)

typedef void* (*OPEN_FUNC) (void);

void print_i2c_reg_setting(struct msm_camera_i2c_reg_setting *setting){
  if(setting != NULL){
    printf(".size = %d \n",setting->size);
    printf(".addr_type = %d \n",setting->addr_type);
    printf(".data_type = %d \n",setting->data_type);
    printf(".delay = %d \n",setting->delay);
    struct msm_camera_i2c_reg_array *tmp = setting->reg_setting;
    if(tmp == NULL){
        printf("No vaild reg_settings array. \n");
        return;
     }
    printf("{<reg_addr>,<reg_data>,<delay>} \n");
    for(int i=0;i<setting->size;i++) {
        if(tmp[i].delay != 0)
          printf("{%#x, %#02x, %#02x}, \n",tmp[i].reg_addr,tmp[i].reg_data,tmp[i].delay);
        else
          printf("{%#x, %#02x}, \n",tmp[i].reg_addr,tmp[i].reg_data);
      }
      printf("end of reg_array \n");
  }else {
    printf("Invaild msm_camera_i2c_reg_setting !\n");
  }
}

void print_sensor_lib_info(sensor_lib_t *sensor_info){
  PRINT_START("sensor_lib_t");
  if(sensor_info->actuator_name != NULL)
    printf(".actuator_name = %s , \n",sensor_info->actuator_name);
  printf(".snapshot_exp_wait_frames = %u , \n",sensor_info->snapshot_exp_wait_frames);
  printf(".sensor_num_frame_skip = %u , \n",sensor_info->sensor_num_frame_skip);
  printf(".sensor_num_HDR_frame_skip = %u , \n",sensor_info->sensor_num_HDR_frame_skip);
  printf(".exposure_table_size = %u , \n",sensor_info->exposure_table_size);
  printf(".sensor_max_immediate_frame_delay = %u , \n",sensor_info->sensor_max_immediate_frame_delay);
  PRINT_END("sensor_lib_t");
}

void print_sensor_slave_info(sensor_lib_t *sensor_info){
  struct msm_camera_sensor_slave_info *sensor_slave_info = sensor_info->sensor_slave_info;
  if(sensor_slave_info == NULL){
    printf("Error : Invaild sensor_slave_info \n");
    return;
  }

  PRINT_START(".sensor_slave_info");
  printf(".camera_id = %d ,\n",sensor_slave_info->camera_id);
  printf(".slave_addr = %#02x ,\n",sensor_slave_info->slave_addr);
  printf(".addr_type = %d ,\n",sensor_slave_info->addr_type);

  printf(".sensor_id_info = { \n");
  struct msm_sensor_id_info_t sensor_id_info = sensor_slave_info->sensor_id_info;
  printf(" .sensor_id_reg_addr = %#04x ,\n ",sensor_id_info.sensor_id_reg_addr);
  printf(" .sensor_id = %#04x ,\n ",sensor_id_info.sensor_id);
  printf("}, \n");

  printf(".power_setting_array = { \n");
  struct msm_sensor_power_setting_array power_setting_array = sensor_slave_info->power_setting_array;
  struct msm_sensor_power_setting *power_setting=power_setting_array.power_setting;
  printf(".power_setting = \n");
  if(power_setting != NULL){
    for(int i=0;i<power_setting_array.size;i++){
      printf("{ \n");
      printf(".seq_type = %d \n",power_setting[i].seq_type);
      printf(".seq_val = %d \n",power_setting[i].seq_val);
      printf(".config_val = %ld \n",power_setting[i].config_val);
      printf(".delay =  %d \n",power_setting[i].delay);
      printf("},\n");
    }
  }else {
    printf("Error : Invaild msm_sensor_power_setting \n");
    return;
  }
  printf(".size = %d \n",power_setting_array.size);
  printf("}, \n");

  printf(".is_flash_supported = %d \n",sensor_slave_info->is_flash_supported);
  PRINT_END(".sensor_slave_info");
}

void print_sensor_init_params(sensor_lib_t *sensor_info){
  PRINT_START(".sensor_init_params");
  struct msm_sensor_init_params *sensor_init_params = sensor_info->sensor_init_params;
  if(sensor_init_params != NULL){
    printf(".modes_supported = %d ,\n",sensor_init_params->modes_supported);
    printf(".position = %d ,\n",sensor_init_params->modes_supported);
    printf(".sensor_mount_angle = %d ,\n",sensor_init_params->sensor_mount_angle);
  }else {
    printf("Invaild msm_sensor_init_params!\n");
  }
  PRINT_END(".sensor_init_params");

  PRINT_START(".sensor_output");
  sensor_output_t *sensor_output = sensor_info->sensor_output;
  if(sensor_output != NULL) {
    printf(".output_format = %d, \n",sensor_output->output_format);
    printf(".connection_mode = %d, \n",sensor_output->connection_mode);
    printf(".raw_output = %d ,\n",sensor_output->raw_output);
  }else {
    printf("Invaild sensor_output_t! \n");
  }
  PRINT_END(".sensor_output");

  PRINT_START(".output_reg_addr");
  struct msm_sensor_output_reg_addr_t *output_reg_addr = sensor_info->output_reg_addr;
  if(output_reg_addr != NULL) {
    printf(".x_output = %#04x, \n",output_reg_addr->x_output);
    printf(".y_output = %#04x, \n",output_reg_addr->y_output);
    printf(".line_length_pclk = %#04x, \n",output_reg_addr->line_length_pclk);
    printf(".frame_length_lines = %#04x, \n",output_reg_addr->frame_length_lines);
  }else {
    printf("Invaild msm_sensor_output_reg_addr_t! \n");
  }
  PRINT_END(".output_reg_addr");

  PRINT_START(".exp_gain_info");
  struct msm_sensor_exp_gain_info_t *exp_gain_info = sensor_info->exp_gain_info;
  if(exp_gain_info != NULL) {
    printf(".coarse_int_time_addr = %#04x, \n",exp_gain_info->coarse_int_time_addr);
    printf(".global_gain_addr = %#04x, \n",exp_gain_info->global_gain_addr);
    printf(".vert_offset = %d, \n",exp_gain_info->vert_offset);
  }else {
    printf("Invaild msm_sensor_exp_gain_info_t! \n");
  }
  PRINT_END(".exp_gain_info");

  PRINT_START(".aec_info");
  sensor_aec_data_t *aec_info = sensor_info->aec_info;
  if(aec_info != NULL){
    printf(".max_gain = %f ,\n",aec_info->max_gain);
    printf(".max_linecount = %d \n",aec_info->max_linecount);
  } else {
    printf("Invaild sensor_aec_data_t ! \n");
  }
  PRINT_END(".aec_info");

  PRINT_START(".default_lens_info");
  sensor_lens_info_t *default_lens_info = sensor_info->default_lens_info;
  if(default_lens_info != NULL){
    printf(".focal_length = %f \n",default_lens_info->focal_length);
    printf(".pix_size =  %f \n",default_lens_info->pix_size);
    printf(".f_number =  %f \n",default_lens_info->f_number);
    printf(".total_f_dist = %f \n",default_lens_info->total_f_dist);
    printf(".hor_view_angle = %f \n",default_lens_info->hor_view_angle);
    printf(".ver_view_angle = %f \n",default_lens_info->ver_view_angle);
  }else {
    printf("Invaild sensor_lens_info_t ! \n");
  }
  PRINT_END(".default_lens_info");

  PRINT_START(".csi_lane_params");
  struct csi_lane_params_t *csi_lane_params = sensor_info->csi_lane_params;
  if(csi_lane_params != NULL) {
    printf(".csi_lane_assign = %#04x \n",csi_lane_params->csi_lane_assign);
    printf(".csi_lane_mask = %#02x \n",csi_lane_params->csi_lane_mask);
    printf(".csi_if = %d \n",csi_lane_params->csi_if);
    printf(".csid_core = %d \n",csi_lane_params->csid_core[0]);
  }else {
    printf("Invaild csi_lane_params_t ! \n");
  }
  PRINT_END(".csi_lane_params");
}

void print_init_settings_array(sensor_lib_t *sensor_info){
  struct sensor_lib_reg_settings_array *init_settings_array = sensor_info->init_settings_array;
  PRINT_START(".init_settings_array");
  if(init_settings_array != NULL){
    struct msm_camera_i2c_reg_setting *setting_array = init_settings_array->reg_settings;
    if(setting_array == NULL) {
      printf("ERROR: No array avaiable.\n");
      PRINT_END(".init_settings_array");
    }
    printf(".size = %d \n",init_settings_array->size);
    for(int i=0;i<init_settings_array->size;i++) {
        printf("******* Begin of %d i2c reg setting*********\n",i);
        print_i2c_reg_setting(&setting_array[i]);
        printf("******* End of %d i2c reg setting*********\n",i);
    }
  }else {
    printf("Invaild sensor_lib_reg_settings_array !\n");
    return;
  }
  PRINT_END(".init_settings_array");
}

void print_settings_array(sensor_lib_t *sensor_info){
  PRINT_START(".start_settings");
  struct msm_camera_i2c_reg_setting *start_settings = sensor_info->start_settings;
  if(start_settings != NULL){
    print_i2c_reg_setting(start_settings);
  }else {
    printf("Invaild start_settings !\n");
  }
  PRINT_END(".start_settings");

  PRINT_START(".stop_settings");
  struct msm_camera_i2c_reg_setting *stop_settings = sensor_info->stop_settings;
  if(stop_settings != NULL){
    print_i2c_reg_setting(stop_settings);
  }else {
    printf("Invaild stop_settings !\n");
  }
  PRINT_END(".stop_settings");

  PRINT_START(".groupon_settings");
  struct msm_camera_i2c_reg_setting *groupon_settings = sensor_info->groupon_settings;
  if(groupon_settings != NULL){
    print_i2c_reg_setting(groupon_settings);
  }else {
    printf("Invaild groupon_settings !\n");
  }
  PRINT_END(".groupon_settings");

  PRINT_START(".groupoff_settings");
  struct msm_camera_i2c_reg_setting *groupoff_settings = sensor_info->groupoff_settings;
  if(groupoff_settings != NULL){
    print_i2c_reg_setting(groupoff_settings);
  }else {
    printf("Invaild groupoff_settings !\n");
  }
  PRINT_END(".groupoff_settings");
}

void print_sensor_lib_reg_settings_array_info(sensor_lib_t *sensor_info){
  if(sensor_info->res_settings_array != NULL){
    struct sensor_lib_reg_settings_array *reg_init_setting = sensor_info->res_settings_array;
    struct msm_camera_i2c_reg_setting *reg_init_setting_array = reg_init_setting->reg_settings;
    printf("Available init arrays : %d\n",reg_init_setting->size);
    for(int i = 0;i<reg_init_setting->size;i++){
      printf("******* Begin of %d i2c reg setting*********\n",i);
      printf(".size = %d \n",reg_init_setting_array[i].size);
      printf(".addr_type = %d \n",reg_init_setting_array[i].addr_type);
      printf(".data_type = %d \n",reg_init_setting_array[i].data_type);
      printf(".delay = %d \n",reg_init_setting_array[i].delay);
      printf("reg_array: \n");
      struct msm_camera_i2c_reg_array *tmp = reg_init_setting_array[i].reg_setting;
      if(tmp == NULL){
        printf("No vaild reg_settings array. \n");
        continue;
      }
      printf("{<reg_addr>,<reg_data>,<delay>} \n");
      for(int j=0;j<reg_init_setting_array[i].size;j++) {
        if(tmp[j].delay != 0)
          printf("{%#x, %#02x, %#02x}, \n",tmp[j].reg_addr,tmp[j].reg_data,tmp[j].delay);
        else
          printf("{%#x, %#02x}, \n",tmp[j].reg_addr,tmp[j].reg_data);
      }
      printf("end of reg_array \n");
      printf("******* End of %d i2c reg setting*********\n",i);
    }
  } else {
    printf("No Vaild i2c init array found! \n");
  }
}

void print_csi_cid_params(sensor_lib_t *sensor_info){
  struct msm_camera_csid_vc_cfg *cid_cfg = sensor_info->csi_cid_params;
  PRINT_START(".csi_cid_params & .csi_cid_params_size");
  if(cid_cfg != NULL){
    printf(".csi_cid_params_size = %d, \n",sensor_info->csi_cid_params_size);
    printf("{<cid>,<dt>,<decode_format>} \n");
    for(int i=0;i<sensor_info->csi_cid_params_size;i++){
      printf("{%d,%d,%d}, \n",cid_cfg[i].cid,cid_cfg[i].dt,cid_cfg[i].decode_format);
    }
  }else {
    printf("Invaild msm_camera_csid_vc_cfg! \n");
  }
  PRINT_END(".csi_cid_params & .csi_cid_params_size");
}

void print_sensor_res_cfg_table(sensor_lib_t *sensor_info) {
    struct sensor_res_cfg_table_t *res_table = sensor_info->sensor_res_cfg_table;
    PRINT_START(".sensor_res_cfg_table");
    if(res_table != NULL){
        sensor_res_cfg_type_t *res_cfg = res_table->res_cfg_type;
        printf(".size = %d \n",res_table->size);
        if(res_cfg != NULL) {
          printf("{ ");
          for(int i=0;i<res_table->size;i++){
            printf("%d, ",res_cfg[i]);
          }
          printf("} \n");
        }else {
          printf("Invaild sensor_res_cfg_type_t !\n");
          return;
        }
    }else {
        printf("Invaild sensor_res_cfg_table_t !\n");
    }
    PRINT_END(".sensor_res_cfg_table");
}

void print_out_info_array(sensor_lib_t *sensor_info){
  struct sensor_lib_out_info_array *out_info_array = sensor_info->out_info_array;
  PRINT_START(".out_info_array");
  if(out_info_array != NULL) {
    struct sensor_lib_out_info_t *sensor_out_info = out_info_array->out_info;
    if(sensor_out_info != NULL) {
      
      printf(".size= %d,\n",out_info_array->size);
      for(int i=0;i<out_info_array->size;i++){
        printf("{ \n");
        printf(" .x_output = %d, \n",sensor_out_info[i].x_output);
        printf(" .y_output = %d, \n",sensor_out_info[i].y_output);
        printf(" .line_length_pclk = %d, \n",sensor_out_info[i].line_length_pclk);
        printf(" .frame_length_lines = %d, \n",sensor_out_info[i].frame_length_lines);
        printf(" .vt_pixel_clk = %u, \n",sensor_out_info[i].vt_pixel_clk);
        printf(" .op_pixel_clk = %u, \n",sensor_out_info[i].op_pixel_clk);
        printf(" .binning_factor = %d, \n",sensor_out_info[i].binning_factor);
        printf(" .max_fps = %f, \n",sensor_out_info[i].max_fps);
        printf(" .min_fps = %f, \n",sensor_out_info[i].min_fps);
        printf(" .mode = %d, \n",sensor_out_info[i].mode);
        printf("}, \n");
      }
      
    }else {
      printf("Invaild sensor_lib_out_info_t! \n");
      return;
    }
  }else {
    printf("Invaild sensor_lib_out_info_array! \n");
    return;
  }
  PRINT_END(".out_info_array");
}

void print_crop_params_array(sensor_lib_t *sensor_info){
  struct sensor_lib_crop_params_array *crop_params_array = sensor_info->crop_params_array;
  PRINT_START(".crop_params_array");
  if(crop_params_array != NULL){
    struct sensor_crop_parms_t *crop_params = crop_params_array->crop_params;
    if(crop_params != NULL){
      printf(".size = %d \n",crop_params_array->size);
      printf("{<top_crop>,<bottom_crop>,<left_crop>,<right_crop>} \n");
      for(int i=0;i<crop_params_array->size;i++){
          printf("{%d,%d,%d,%d},\n ",crop_params[i].top_crop,crop_params[i].bottom_crop
                ,crop_params[i].left_crop,crop_params[i].right_crop);
      }
    }else {
      printf("Invaild sensor_crop_parms_t! \n");
    }
  }else {
    printf("Invaild sensor_lib_crop_params_array! \n");
  }
  PRINT_END(".crop_params_array");
}

void print_csi_params_array(sensor_lib_t *sensor_info){
  struct sensor_lib_csi_params_array *csi_params_array = sensor_info->csi_params_array;
  PRINT_START(".csi_params_array");
  if(csi_params_array != NULL){
    struct msm_camera_csi2_params **csi_params = csi_params_array->csi2_params;
    if(csi_params != NULL) {
      printf(".size = %d \n",csi_params_array->size);
      for(int i=0;i<csi_params_array->size;i++){
        printf("******* Begin of %d msm_camera_csi2_params*********\n",i);
          struct msm_camera_csi2_params *tmp = csi_params[i];
        printf(".csid_params = { \n");
        printf("\t.lane_cnt = %d, \n",tmp->csid_params.lane_cnt);
        printf("\t.lut_params = { \n");
        printf("\t .num_cid = %d \n",tmp->csid_params.lut_params.num_cid);
        printf("\t .vc_cfg = { \n");
        printf("\t\t {<cid>,<dt>,<decode_format>} \n");
        struct msm_camera_csid_vc_cfg *vc_cfg = *tmp->csid_params.lut_params.vc_cfg;
        for(int i=0;i<tmp->csid_params.lut_params.num_cid;i++){
            struct msm_camera_csid_vc_cfg cid_cfg = vc_cfg[i];
            printf("\t\t {%d, %d, %d}, \n",cid_cfg.cid,cid_cfg.dt,cid_cfg.decode_format);
          }
        printf("\t }, \n");
        printf("\t}, \n");
        printf("}, \n");
        printf(".csiphy_params = { \n");
        printf("\t.lane_cnt = %d, \n",tmp->csiphy_params.lane_cnt);
        printf("\t.settle_cnt = %#02x, \n",tmp->csiphy_params.settle_cnt);
        printf("}, \n");
      printf("******* End of %d msm_camera_csi2_params*********\n",i);
    }
    } else {
      printf("Invaild msm_camera_csi2_params ! \n");
    }
  }else {
    printf("Invaild csi_params_array ! \n");
  }
  PRINT_END(".csi_params_array");
}

void print_sensor_stream_info(sensor_lib_t *sensor_info) {
  sensor_stream_info_array_t *stream_info_array = sensor_info->sensor_stream_info_array;
  PRINT_START(".sensor_stream_info_array");
  if(stream_info_array != NULL){
     sensor_stream_info_t *stream_info = stream_info_array->sensor_stream_info;
     if(stream_info != NULL) {
        printf("\t .size = %d \n",stream_info_array->size);
        for(int i=0;i<stream_info_array->size;i++){
          sensor_stream_info_t tmp = stream_info[i];
          printf("******* Begin of %d sensor_stream_info_t*********\n",i);
          printf("\t {");
          printf("\t\t .vc_cfg_size = %d \n",tmp.vc_cfg_size);
          printf("\t\t .vc_cfg = { \n");
          printf("\t\t\t {<cid>,<dt>,<decode_format>} \n");
          struct msm_camera_csid_vc_cfg *vc_cfg = tmp.vc_cfg;
          for(int i=0;i<tmp.vc_cfg_size;i++){
            struct msm_camera_csid_vc_cfg cid_cfg = vc_cfg[i];
            printf("\t\t\t {%d, %d, %d}, \n",cid_cfg.cid,cid_cfg.dt,cid_cfg.decode_format);
          }
          printf("\t\t }, \n");
          struct sensor_pix_fmt_info_t *pix_fmt_fourcc = tmp.pix_fmt_fourcc;
          printf("\t\t .pix_fmt_fourcc = %d \n",pix_fmt_fourcc[0].fourcc);
          printf("\t },");
          printf("******* End of %d sensor_stream_info_t*********\n",i);
        }
     } else {
        printf("Invaild sensor_stream_info_t ! \n");
     }
  }else {
    printf("Invaild sensor_stream_info_array_t ! \n");
  }
  PRINT_END(".sensor_stream_info_array");
}

int main(int argc,char** args) {
  if(argc != 2){
    printf("Usage : qcdg <path to libmmcamera_XXX.so> \n");
    return 0;
  }

  char *cam_name = malloc(sizeof(char) * 50);
  void *handle;
  sscanf(args[1],"%*[^_]_%[^.]",cam_name);

  handle = dlopen(args[1],RTLD_NOW);
  if(!handle) {
    printf("Error : %s \n",dlerror());
    return -1;
  }

  dlerror();

  /* Assume all sensor driver open function named <sensor_name>_open_lib */
  char *func_name = malloc(sizeof(char) * 100);
  sprintf(func_name,"%s_open_lib",cam_name);

  OPEN_FUNC open_func = NULL;
  char *err;
  open_func = (OPEN_FUNC)dlsym(handle,func_name);
  if((err = dlerror()) != NULL){
    printf("ERROR: %s \n",err);
    return -1;
  }

  sensor_lib_t *sensor_info = (sensor_lib_t *)open_func();
  if(sensor_info == NULL){
    printf("Invaild init function! Exiting...\n");
    return -1;
  }

  PRINT_START(cam_name);
  print_sensor_lib_info(sensor_info);
  print_sensor_slave_info(sensor_info);
  print_sensor_init_params(sensor_info);
  print_csi_cid_params(sensor_info);
  print_init_settings_array(sensor_info);
  print_settings_array(sensor_info);
  print_sensor_res_cfg_table(sensor_info);
  print_sensor_lib_reg_settings_array_info(sensor_info);
  print_out_info_array(sensor_info);
  print_crop_params_array(sensor_info);
  print_csi_params_array(sensor_info);
  print_sensor_stream_info(sensor_info);
  PRINT_END(cam_name);

  dlclose(handle);
  return 0;
}