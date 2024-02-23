from utils.TieThread import *
from io import StringIO
import time
import subprocess
import socket

def try_create_sk(addr, timing=1, count_try=None, debug=False):
	is_connect = False
	while not is_connect:
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try:
			is_connect = True
			sock.settimeout(timing)
			sock.connect(addr)
			sock.settimeout(None)
		except:
			is_connect = False
			if debug:
				print(f"Not connected to {addr[0]}:{addr[1]}, try again...")
		if is_connect:
			if debug:
				print(f"Connected to {addr[0]}:{addr[1]}")
			return sock
		if count_try != None: 
			count_try -= 1
			if count_try == 0: 
				if debug:
					print("End of trying connect")
				return None
		time.sleep(timing)

def get_def_gw_ip():
	message = ""

	###Try and process output command to get ip of gateway
	try:
		output = subprocess.check_output("ipconfig", shell=True)
	except:
		message = "Command error"
		return message, None
	
	list_shell = StringIO(output.decode()).readlines()
	ip_gateway = None
	for i in range(len(list_shell)):
		if "Wireless LAN adapter Wi-Fi" in list_shell[i]:
			for j in range(i+1, min(i+10, len(list_shell))):
				if "IPv4 Address" in list_shell[j]:
					ip_gateway = list_shell[j].split(':')[1].strip()
					i = len(list_shell)
					break
			i += 10
	if ip_gateway == None:
		message = "No exist ip of gateway"
		return message, None
	
	message = "[IP-DefaultGW] -- Got ip: " + ip_gateway
	return message, ip_gateway

def thread_scan_ip_esp(this, props):
	###split ip default
	ip_gateway, ip_store, port_def, range_scan = props
	print(f"[T-{this['id']}] -- Start scanning available tcpip neighbor...")
	temp_ip_gw = ip_gateway
	while temp_ip_gw[-1] != '.': temp_ip_gw = temp_ip_gw[:-1]
	
	list_ip_neighbor = []
	for last_num_ip in range(range_scan[0], range_scan[1]+1):
		host = temp_ip_gw + str(last_num_ip)
		client = try_create_sk((host, port_def), count_try=1, timing=0.1)
		if client != None:
			list_ip_neighbor.append(host)
	print(f"[T-{this['id']}] -- Done scanning: ", list_ip_neighbor)
	if len(list_ip_neighbor) > 0: ip_store.extend(list_ip_neighbor)

def scan_tcpip_main(port_def):
	threads = ThreadRoster(debug=False)
	ret, ip_gw = get_def_gw_ip()
	print(ret)
	ip_store = []

	threads.create(thread_scan_ip_esp, 'scan_tcp', [ip_gw, ip_store, port_def, (2, 20)])
	segment = 20
	while segment < 240: 
		threads.create(thread_scan_ip_esp, 'scan_tcp', [ip_gw, ip_store, port_def, (segment+1, segment+20)])
		segment += 20
	threads.create(thread_scan_ip_esp, 'scan_tcp', [ip_gw, ip_store, port_def, (241, 254)])

	while threads.thread_alive('scan_tcp'):
		time.sleep(0.5)
	print("Done all scan threads, got: ", ip_store)
	threads.close_all()
	return ip_store

#scan_tcpip_main(6123)