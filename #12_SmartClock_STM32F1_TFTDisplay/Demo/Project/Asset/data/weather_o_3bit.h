//Color: 2177, 3840, 17670, 24393, 51199, 64201, 64648, 65451
//weather_o_ANIM 8x8
//!24!-Length
static const uint8_t weather_o_arr[1][24] = {
    { 60, 126, 195, 195, 195, 195, 126, 60, 
      60, 126, 195, 195, 195, 195, 126, 60, 
      60, 126, 195, 195, 195, 195, 126, 60, 
    },
};

static const Tie_BMP_DATA weather_o_data[] = {
    { weather_o_arr[0] }, 
};

static const Tie_ANIM weather_o_anim = { 8, 8, 1, weather_o_data };
