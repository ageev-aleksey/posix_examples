import os
import sys
import socket

if __name__ == "__main__":
	if len(sys.argv) != 4:
		print (f"Usage: {sys.argv[0]} <number_clients> <server_ip> <server_port>")
		sys.exit(-1)
	num_clients = int(sys.argv[1])
	ip = sys.argv[2]
	port = int(sys.argv[3])
	children = []
	is_parent = True
	for i in range(num_clients - 1):
		pid = os.fork();
		if pid == 0:
			is_parent = False
			break;
		else:
			children.append(pid)
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
	s.settimeout(2);
	try:
		s.connect((ip, port));
		s.send(("Hello from python " + str(os.getpid())).encode("ASCII"))
		res = s.recv(500);
		print(res)
	except:
		print("TimeOut!");
		s.close()
		sys.exit(1);

	if is_parent:
		timeouts = 0
		for i in range(len(children)):
			code = os.wait()
			if code[1] == 256:
				timeouts = timeouts + 1
		print(f"Successful; timeouts: {timeouts}")