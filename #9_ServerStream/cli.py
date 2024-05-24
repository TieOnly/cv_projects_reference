import socket
import time
import pickle
import struct

def enum(*sequential, **named):
    enums = dict(zip(sequential, range(len(sequential))), **named)
    return type('Enum', (), enums)

DT = enum (
	'STR',
	'SERIES',
	'CMD',
)

CMD = enum (
	'NONE',

	'CAM_CLI_BEG',
	'CAM_SER_TRANS',
	'CAM_SER_PAUSE',
)

def tcp_get_from_recv(data):
	data_len, data_type = struct.unpack("<LL", data[:8])
	print(f"Struct: {data_len}, {data_type}")
	data = data[8:]
	if data_type == 0:
		return data_type, data.decode(), data_len
	if data_type == 1:
		return data_type, pickle.loads(data, fix_imports=True, encoding="bytes"), data_len
	if data_type == 2:
		return data_type, struct.unpack("<L", data)[0], data_len
	pass

def tcp_send_pack(data_type, data, data_len):
	data_desc = struct.pack("<LL", data_len, data_type)
	data_dump = data
	if   data_type == DT.STR:
		data_dump = data.encode()
	elif data_type == DT.SERIES:
		data_dump = pickle.dumps(data)
	else:
		data_dump = struct.pack("<L", data)
	return data_desc+data_dump

def main():
	# host = '192.168.137.68'  # Địa chỉ IP của máy chủ
	host = '127.0.0.1'
	data_send = 'task-cam'
	payload = [1, 2, 3, 4, 5,1, 2, 3, 4, 5,1, 2, 3, 4, 5, 1, 2, 3, 4, 5]
	# payload = '123'
	port = 6123  # Cổng để kết nối

	client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	client_socket.connect((host, port))
	print("Client start")
	try: client_socket.sendall(data_send.encode())
	except: 
		client_socket.close()
		return

	try: data_recv = client_socket.recv(1024)
	except: 
		client_socket.close()
		return
	
	if data_recv == b'result register: success':
		print("Client register success")
		data_send = tcp_send_pack(DT.CMD, CMD.CAM_CLI_BEG, 1)
	else:
		print("Client register failed")
		client_socket.close()
		return 


	while True:
		try:
			client_socket.sendall(data_send)
		except: break

		try:
			data_recv = client_socket.recv(1024)
		except: break
		print('Server:', data_recv)

		# pl = tcp_get_from_recv(data_recv)
		# print(pl)
  
		data_type, data, data_len = tcp_get_from_recv(data_recv)
		print(f"Pack: {data_type}, {data_len}, {data}")

		if data_type == DT.CMD:
			print(f"DATA-CMD: {data}")
			if data == CMD.CAM_SER_TRANS:
				print(f"DATA-CMD: stream {data}")

		if data_type == DT.STR:
			print(f"DATA-STR: {data}")

		if data_type == DT.SERIES:
			print(f"DATA-SERIES: {data}")

		print(f"Send: {data_send}")
		time.sleep(1)
		
	# Đóng kết nối
	client_socket.close()

if __name__ == '__main__':
    main()