###Các đầu mục
- src: chứa mã nguồn project
- build: chứa các file thực thi và firmware cho phần cứng

- sink node: chứa các file liên quan đến phần nodemcu đóng vai trò server/trạm quản lý máy khách, dữ liệu
- client node: chứa các file liên quan đến phần nodemcu đóng vai trò là máy khách và hiển thị dữ liệu 
- app client: chứa các file liên quan đến phần client chạy trên pc, vai trò như máy khách

###Source
- Mô tả các phương pháp giải quyết bài toán bằng code, thuật toán
- Bao gồm các file:
	.py: mã nguồn python, viết cho các client chạy trên pc
	.c, .h: mã nguồn c, viết cho các nodemcu, driver cho phần cứng

###Build
- Chứa các file thực thi như .exe, các file firmware .bin, .elf, .map
- Các file .exe chạy trực tiếp trên pc chạy hệ điều hành Window
- Các file .bin, .elf thực thi trên nodemcu qua các bước nạp firmware

###Setup
#NodeMCU:
- Ít nhất 2 esp8266, 1 làm sinknode 1 làm client, màn hình LCD5110
- Lắp đặt phần cứng theo define trong mã nguồn
- Cài đặt môi trường và SDK theo hướng dẫn tại https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html 
- Build: build mã nguồn hoặc nạp firmware bằng SDK
	.make build: build các file trong mã nguồn
	.make flash: nạp firmware đã build
#App
- Thực thi file .exe trong folder build
- Hoặc thực thi các file mã nguồn bằng python đã cài các module:
	.socket, base64, cv2
	. nếu chưa, thực hiện cài bằng tool pip

###Test
- Cấp nguồn cho sink node, ấn reset để đảm bảo nodemcu khởi chạy
- Cấp nguồn cho node client, ấn reset để đảm bảo nodemcu và màn hình khởi chạy
- Tại máy tính có webcam, kết nối vào địa chỉ broadcast ip mà sinknode cung cấp, cụ thể "tieonly"
- Khởi chạy client-stream-src.exe:
	.Chạy trên cmd: client-stream-src.src.exe [n1] [n2]
	.[n1]: chọn chế độ, bao gồm: webcam hoặc video
	.[n2]: chọn ip
	.eg: client-stream-src.exe webcam 192.168.4.1
--> Tại đây quan sát kết quả trên pc, nodemcu

- Khởi chạy client-stream-view để quan sát trên pc
- Ấn nút cứng flash trên sinknode để chọn client muốn hiển thị