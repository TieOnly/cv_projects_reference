#pragma once
// //Pika
// const unsigned char ani_count = 4;
// const unsigned char ani_w = 48;
// const unsigned char ani_h = 48;
// const unsigned char ani[4][288] = {
//    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 129, 143, 255, 255, 255, 255, 216, 97, 255, 255, 255, 255, 221, 12, 255, 255, 255, 255, 199, 192, 255, 255, 255, 255, 143, 248, 255, 255, 255, 255, 167, 248, 255, 255, 255, 255, 128, 98, 255, 255, 255, 255, 175, 152, 255, 255, 255, 255, 248, 52, 127, 255, 255, 255, 254, 176, 127, 129, 255, 255, 254, 28, 254, 132, 63, 255, 254, 2, 16, 0, 159, 255, 254, 48, 130, 0, 71, 255, 255, 0, 0, 7, 17, 255, 255, 224, 19, 138, 12, 63, 255, 229, 15, 227, 8, 159, 255, 240, 15, 177, 250, 95, 255, 252, 95, 189, 127, 143, 255, 254, 31, 255, 223, 175, 255, 255, 183, 255, 251, 175, 255, 255, 191, 205, 182, 175, 255, 255, 159, 223, 252, 39, 255, 255, 142, 255, 162, 87, 255, 255, 31, 198, 240, 71, 255, 255, 79, 119, 241, 215, 255, 255, 95, 231, 231, 135, 255, 255, 75, 255, 127, 47, 255, 255, 177, 211, 156, 79, 255, 255, 221, 11, 3, 31, 255, 255, 224, 40, 188, 127, 255, 255, 240, 46, 15, 255, 255, 255, 255, 172, 95, 255, 255, 255, 255, 160, 159, 255, 255, 255, 255, 128, 63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
//    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 7, 255, 255, 255, 255, 255, 113, 255, 255, 255, 255, 255, 4, 31, 255, 255, 255, 255, 72, 207, 255, 255, 255, 255, 126, 15, 255, 255, 255, 255, 94, 143, 255, 129, 255, 255, 63, 207, 255, 128, 31, 255, 37, 207, 255, 128, 143, 255, 0, 143, 252, 8, 43, 255, 6, 163, 252, 3, 8, 31, 250, 131, 252, 38, 128, 175, 250, 215, 252, 7, 246, 31, 248, 16, 97, 64, 223, 71, 249, 212, 8, 22, 93, 215, 248, 64, 17, 255, 239, 199, 255, 65, 11, 211, 255, 67, 255, 8, 31, 255, 253, 43, 255, 8, 47, 255, 249, 35, 255, 232, 125, 223, 244, 163, 255, 225, 110, 223, 244, 179, 255, 253, 63, 255, 251, 75, 255, 253, 127, 239, 255, 147, 255, 253, 126, 191, 127, 39, 255, 253, 63, 239, 192, 143, 255, 252, 61, 248, 208, 31, 255, 254, 191, 144, 96, 255, 255, 254, 67, 137, 114, 255, 255, 255, 15, 192, 164, 255, 255, 255, 135, 142, 9, 255, 255, 255, 146, 30, 3, 255, 255, 255, 193, 15, 3, 255, 255, 255, 244, 15, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
//    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 15, 255, 255, 255, 255, 255, 193, 255, 255, 255, 255, 255, 12, 127, 255, 255, 255, 255, 97, 15, 255, 255, 255, 255, 250, 55, 255, 192, 255, 255, 59, 151, 255, 192, 15, 255, 119, 87, 254, 0, 35, 255, 63, 151, 254, 0, 4, 63, 8, 129, 254, 6, 1, 143, 6, 33, 254, 3, 240, 51, 0, 65, 255, 8, 185, 129, 249, 245, 252, 3, 31, 227, 248, 0, 112, 225, 143, 245, 249, 196, 36, 5, 255, 187, 255, 0, 1, 175, 251, 216, 255, 24, 7, 255, 255, 147, 255, 0, 183, 247, 255, 9, 255, 8, 125, 255, 255, 153, 255, 224, 255, 231, 125, 25, 255, 230, 95, 255, 223, 56, 255, 226, 255, 255, 251, 179, 255, 248, 255, 255, 222, 64, 255, 253, 231, 255, 255, 136, 255, 253, 191, 255, 239, 226, 255, 250, 247, 238, 133, 114, 255, 245, 189, 240, 216, 1, 255, 228, 127, 7, 71, 243, 255, 193, 252, 112, 112, 3, 255, 210, 227, 7, 255, 255, 255, 208, 8, 255, 255, 255, 255, 208, 225, 255, 255, 255, 255, 208, 15, 255, 255, 255, 255, 192, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
//    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 135, 31, 255, 255, 255, 255, 152, 67, 255, 255, 255, 255, 158, 26, 255, 255, 255, 255, 29, 80, 255, 255, 255, 255, 31, 218, 255, 255, 255, 255, 95, 250, 255, 255, 255, 255, 27, 244, 63, 255, 255, 255, 128, 20, 63, 255, 255, 255, 255, 216, 63, 255, 255, 255, 128, 10, 15, 196, 255, 255, 255, 0, 0, 3, 31, 255, 255, 58, 4, 128, 71, 255, 255, 232, 48, 0, 49, 255, 255, 224, 17, 194, 77, 127, 255, 224, 11, 225, 67, 31, 255, 252, 61, 147, 224, 79, 255, 253, 63, 249, 253, 47, 255, 248, 255, 124, 119, 135, 255, 250, 127, 251, 223, 199, 255, 250, 255, 255, 251, 71, 255, 250, 255, 255, 187, 19, 255, 250, 223, 111, 191, 83, 255, 248, 255, 255, 176, 3, 255, 252, 247, 249, 248, 243, 255, 244, 247, 255, 248, 99, 255, 244, 225, 63, 235, 239, 255, 242, 1, 131, 191, 147, 255, 240, 56, 98, 188, 39, 255, 242, 67, 11, 192, 143, 255, 240, 31, 225, 210, 31, 255, 255, 255, 248, 202, 127, 255, 255, 255, 240, 232, 127, 255, 255, 255, 248, 97, 127, 255, 255, 255, 253, 1, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, },
// };

const uint8_t font_count = 96;
const uint8_t font_w = 8;
const uint8_t font_h = 14;
const unsigned char font[96][14] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 24, 60, 60, 60, 24, 24, 0, 24, 24, 0, 0, 0, },
    { 0, 0, 102, 102, 102, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 54, 54, 127, 54, 54, 54, 127, 54, 54, 0, 0, 0, },
    { 24, 24, 60, 102, 96, 48, 24, 12, 6, 102, 60, 24, 24, 0, },
    { 0, 112, 216, 218, 118, 12, 24, 48, 110, 91, 27, 14, 0, 0, },
    { 0, 0, 56, 108, 108, 56, 96, 111, 102, 102, 59, 0, 0, 0, },
    { 0, 0, 24, 24, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 12, 24, 24, 48, 48, 48, 48, 48, 24, 24, 12, 0, },
    { 0, 0, 48, 24, 24, 12, 12, 12, 12, 12, 24, 24, 48, 0, },
    { 0, 0, 0, 0, 54, 28, 127, 28, 54, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 24, 24, 126, 24, 24, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 28, 12, 24, 0, },
    { 0, 0, 0, 0, 0, 0, 126, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 28, 0, 0, 0, },
    { 0, 0, 6, 6, 12, 12, 24, 24, 48, 48, 96, 96, 0, 0, },
    { 0, 0, 30, 51, 55, 55, 51, 59, 59, 51, 30, 0, 0, 0, },
    { 0, 0, 12, 28, 124, 12, 12, 12, 12, 12, 12, 0, 0, 0, },
    { 0, 0, 60, 102, 102, 6, 12, 24, 48, 96, 126, 0, 0, 0, },
    { 0, 0, 60, 102, 102, 6, 28, 6, 102, 102, 60, 0, 0, 0, },
    { 0, 0, 48, 48, 54, 54, 54, 102, 127, 6, 6, 0, 0, 0, },
    { 0, 0, 126, 96, 96, 96, 124, 6, 6, 12, 120, 0, 0, 0, },
    { 0, 0, 28, 24, 48, 124, 102, 102, 102, 102, 60, 0, 0, 0, },
    { 0, 0, 126, 6, 12, 12, 24, 24, 48, 48, 48, 0, 0, 0, },
    { 0, 0, 60, 102, 102, 118, 60, 110, 102, 102, 60, 0, 0, 0, },
    { 0, 0, 60, 102, 102, 102, 102, 62, 12, 24, 56, 0, 0, 0, },
    { 0, 0, 0, 0, 28, 28, 0, 0, 0, 28, 28, 0, 0, 0, },
    { 0, 0, 0, 0, 28, 28, 0, 0, 0, 28, 28, 12, 24, 0, },
    { 0, 0, 6, 12, 24, 48, 96, 48, 24, 12, 6, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 126, 0, 126, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 96, 48, 24, 12, 6, 12, 24, 48, 96, 0, 0, 0, },
    { 0, 0, 60, 102, 102, 12, 24, 24, 0, 24, 24, 0, 0, 0, },
    { 0, 0, 126, 195, 195, 207, 219, 219, 207, 192, 127, 0, 0, 0, },
    { 0, 0, 24, 60, 102, 102, 102, 126, 102, 102, 102, 0, 0, 0, },
    { 0, 0, 124, 102, 102, 102, 124, 102, 102, 102, 124, 0, 0, 0, },
    { 0, 0, 60, 102, 102, 96, 96, 96, 102, 102, 60, 0, 0, 0, },
    { 0, 0, 120, 108, 102, 102, 102, 102, 102, 108, 120, 0, 0, 0, },
    { 0, 0, 126, 96, 96, 96, 124, 96, 96, 96, 126, 0, 0, 0, },
    { 0, 0, 126, 96, 96, 96, 124, 96, 96, 96, 96, 0, 0, 0, },
    { 0, 0, 60, 102, 102, 96, 96, 110, 102, 102, 62, 0, 0, 0, },
    { 0, 0, 102, 102, 102, 102, 126, 102, 102, 102, 102, 0, 0, 0, },
    { 0, 0, 60, 24, 24, 24, 24, 24, 24, 24, 60, 0, 0, 0, },
    { 0, 0, 6, 6, 6, 6, 6, 6, 102, 102, 60, 0, 0, 0, },
    { 0, 0, 102, 102, 108, 108, 120, 108, 108, 102, 102, 0, 0, 0, },
    { 0, 0, 96, 96, 96, 96, 96, 96, 96, 96, 126, 0, 0, 0, },
    { 0, 0, 99, 99, 119, 107, 107, 107, 99, 99, 99, 0, 0, 0, },
    { 0, 0, 99, 99, 115, 123, 111, 103, 99, 99, 99, 0, 0, 0, },
    { 0, 0, 60, 102, 102, 102, 102, 102, 102, 102, 60, 0, 0, 0, },
    { 0, 0, 124, 102, 102, 102, 124, 96, 96, 96, 96, 0, 0, 0, },
    { 0, 0, 60, 102, 102, 102, 102, 102, 102, 102, 60, 12, 6, 0, },
    { 0, 0, 124, 102, 102, 102, 124, 108, 102, 102, 102, 0, 0, 0, },
    { 0, 0, 60, 102, 96, 48, 24, 12, 6, 102, 60, 0, 0, 0, },
    { 0, 0, 126, 24, 24, 24, 24, 24, 24, 24, 24, 0, 0, 0, },
    { 0, 0, 102, 102, 102, 102, 102, 102, 102, 102, 60, 0, 0, 0, },
    { 0, 0, 102, 102, 102, 102, 102, 102, 102, 60, 24, 0, 0, 0, },
    { 0, 0, 99, 99, 99, 107, 107, 107, 54, 54, 54, 0, 0, 0, },
    { 0, 0, 102, 102, 52, 24, 24, 44, 102, 102, 102, 0, 0, 0, },
    { 0, 0, 102, 102, 102, 102, 60, 24, 24, 24, 24, 0, 0, 0, },
    { 0, 0, 126, 6, 6, 12, 24, 48, 96, 96, 126, 0, 0, 0, },
    { 0, 0, 60, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 60, },
    { 0, 0, 96, 96, 48, 48, 24, 24, 12, 12, 6, 6, 0, 0, },
    { 0, 0, 60, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 60, },
    { 24, 60, 102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, },
    { 56, 24, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 60, 6, 6, 62, 102, 102, 62, 0, 0, 0, },
    { 0, 0, 96, 96, 124, 102, 102, 102, 102, 102, 124, 0, 0, 0, },
    { 0, 0, 0, 0, 60, 102, 96, 96, 96, 102, 60, 0, 0, 0, },
    { 0, 0, 6, 6, 62, 102, 102, 102, 102, 102, 62, 0, 0, 0, },
    { 0, 0, 0, 0, 60, 102, 102, 126, 96, 96, 60, 0, 0, 0, },
    { 0, 0, 30, 48, 48, 48, 126, 48, 48, 48, 48, 0, 0, 0, },
    { 0, 0, 0, 0, 62, 102, 102, 102, 102, 102, 62, 6, 6, 124, },
    { 0, 0, 96, 96, 124, 102, 102, 102, 102, 102, 102, 0, 0, 0, },
    { 0, 24, 24, 0, 120, 24, 24, 24, 24, 24, 126, 0, 0, 0, },
    { 0, 12, 12, 0, 60, 12, 12, 12, 12, 12, 12, 12, 12, 120, },
    { 0, 0, 96, 96, 102, 102, 108, 120, 108, 102, 102, 0, 0, 0, },
    { 0, 0, 120, 24, 24, 24, 24, 24, 24, 24, 126, 0, 0, 0, },
    { 0, 0, 0, 0, 126, 107, 107, 107, 107, 107, 99, 0, 0, 0, },
    { 0, 0, 0, 0, 124, 102, 102, 102, 102, 102, 102, 0, 0, 0, },
    { 0, 0, 0, 0, 60, 102, 102, 102, 102, 102, 60, 0, 0, 0, },
    { 0, 0, 0, 0, 124, 102, 102, 102, 102, 102, 124, 96, 96, 96, },
    { 0, 0, 0, 0, 62, 102, 102, 102, 102, 102, 62, 6, 6, 6, },
    { 0, 0, 0, 0, 102, 110, 112, 96, 96, 96, 96, 0, 0, 0, },
    { 0, 0, 0, 0, 62, 96, 96, 60, 6, 6, 124, 0, 0, 0, },
    { 0, 0, 48, 48, 126, 48, 48, 48, 48, 48, 30, 0, 0, 0, },
    { 0, 0, 0, 0, 102, 102, 102, 102, 102, 102, 62, 0, 0, 0, },
    { 0, 0, 0, 0, 102, 102, 102, 102, 102, 60, 24, 0, 0, 0, },
    { 0, 0, 0, 0, 99, 107, 107, 107, 107, 54, 54, 0, 0, 0, },
    { 0, 0, 0, 0, 102, 102, 60, 24, 60, 102, 102, 0, 0, 0, },
    { 0, 0, 0, 0, 102, 102, 102, 102, 102, 102, 60, 12, 24, 240, },
    { 0, 0, 0, 0, 126, 6, 12, 24, 48, 96, 126, 0, 0, 0, },
    { 0, 0, 12, 24, 24, 24, 48, 96, 48, 24, 24, 24, 12, 0, },
    { 0, 0, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, },
    { 0, 0, 48, 24, 24, 24, 12, 6, 12, 24, 24, 24, 48, 0, },
    { 0, 0, 113, 219, 142, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};
