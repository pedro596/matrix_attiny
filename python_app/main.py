import serial
import time

#serial stuff
ser = serial.Serial()

ser.baudrate = 115200
ser.bytesize = serial.EIGHTBITS #number of bits per bytes
ser.parity   = serial.PARITY_NONE #set parity check: no parity
ser.stopbits = serial.STOPBITS_ONE #number of stop bits		
ser.timeout  = 1


r = {
0:int('0b00000111',2),
1:int('0b00000001',2),
2:int('0b00000001',2),
3:int('0b00000001',2),
4:int('0b00000001',2),
5:int('0b00000001',2),
6:int('0b00000001',2),
7:int('0b00000001',2)
}

g = {
0:int('0b00000001',2),
1:int('0b00000001',2),
2:int('0b00000001',2),
3:int('0b00000001',2),
4:int('0b00000001',2),
5:int('0b00000001',2),
6:int('0b00000001',2),
7:int('0b00000001',2)
}

b = {
0:int('0b00000001',2),
1:int('0b00000001',2),
2:int('0b00000001',2),
3:int('0b00000001',2),
4:int('0b00000001',2),
5:int('0b00000001',2),
6:int('0b00000001',2),
7:int('0b00000001',2)
}


ser.port = "/dev/ttyUSB0"

ser.open()

matriz = {0:r, 1:g, 2:b};


while True:
	pedido = ser.readline()

	#find my control char
	try:
		request_start = pedido.index('-')
		request_start = request_start + 1 		#offset for remove the control char
		request_end = request_start +2			#offset for remove the second (in the end) control char
	except ValueError:							#just in case the control char cannot be found
		ser.write('1')							#in this case just send a '1' to unlock matrix soft. and try read again
		continue

	print pedido[request_start:request_end]		#so, if everything goes ok lets print it just for debug 
	resposta = chr(matriz[int(pedido[request_start:(request_end-1)])][int(pedido[request_start+1:request_end])])
	time.sleep(0.0001)
	ser.write(resposta)
	time.sleep(0.0001)
	print ord(resposta)


	'''pedido = pedido[1:3]
	print pedido[:2]
	if pedido[:1][1:2] in matriz:
		ser.write(chr(matriz[str(pedido[:1])][int(pedido[1:2])]))
	else:
		ser.write('1')
		print 'ERRO'
	#print pedido
	
	
	'''