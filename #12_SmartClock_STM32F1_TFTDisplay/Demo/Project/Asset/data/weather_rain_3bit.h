//Color: 2177, 3840, 17670, 24393, 51199, 64201, 64648, 65451
//weather_rain_ANIM 24x24
//!72!-Length
static const uint8_t weather_rain_arr[2][72] = {
    { 8, 16, 64, 16, 32, 128, 17, 36, 128, 34, 73, 8, 34, 65, 8, 68, 130, 16, 68, 128, 16, 129, 0, 32, 
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
      247, 239, 191, 239, 223, 127, 238, 219, 127, 221, 182, 247, 221, 190, 247, 187, 125, 239, 187, 127, 239, 126, 255, 223, 
    },

    { 0, 0, 2, 0, 2, 4, 1, 36, 4, 0, 64, 8, 34, 65, 8, 64, 130, 16, 68, 18, 0, 9, 36, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
      255, 255, 253, 255, 253, 251, 254, 219, 251, 255, 191, 247, 221, 190, 247, 191, 125, 239, 187, 237, 255, 246, 219, 255, 
    },
};

static const Tie_BMP_DATA weather_rain_data[] = {
    { weather_rain_arr[0] }, 
    { weather_rain_arr[1] }, 
};

static const Tie_ANIM weather_rain_anim = { 24, 24, 2, weather_rain_data };