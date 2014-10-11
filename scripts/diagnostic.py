#!/usr/bin/python 

import serial
import crcmod
import struct
import json
import time
import os
from optparse import OptionParser
import csv
import signal
import sys

class SerialConnection:
	def openConnection(self,port,baudrate):
		self.ser = serial.Serial(port,baudrate,timeout=0.25,writeTimeout=0)
		time.sleep(1.0)
		if self.ser.isOpen():
			print "Serial port opened."
		
	def read(self,length):
		preamble = ['\xFF','\xFA'];
		input = [0x00,0x00]
		input[1] = self.ser.read(1)
		while ( True ):
			if ( input == preamble ):
				break
			else:
				input[0] = input[1]
				input[1] = self.ser.read(1)
		
		while ( self.ser.inWaiting() < length + 2 ):
			pass

		#Get data
		data = self.ser.read(length)

		#Get checksum
		input = self.ser.read(2)
		checksum = struct.unpack('H',input)[0]

		#Verify checksum
		crc16 = crcmod.mkCrcFun(0x11021,0xFFFF,True)
		#crc16 = crcmod.mkCrcFun(0x11021,0xFFFF,False) # xmodem
		calcChecksum = crc16(data)
		calcChecksum = (~calcChecksum) % 2**16  # convert to uint16_t
		#calcChecksum = (calcChecksum) % 2**16 # xmodem

		if ( checksum != calcChecksum ):
			print "Failed checksum."
			return
		      
		return data

def sigint_handler(*args):
	print "Closing serial port."
	sc.ser.close()
	print "Quiting gracefully."
	time.sleep(0.25)
	sys.exit()

sc = SerialConnection()

parser = OptionParser()
parser.add_option("-o","--output",dest="filename",help="Output file name.",metavar="FILE")
parser.add_option("-f","--format",dest="formatFile",help="Format JSON file.")
parser.add_option("-p","--port",dest="portname")
(options,args) = parser.parse_args()

isFile = False
if options.filename:
	csvfile = open(options.filename,'w')
	csvwriter = csv.writer(csvfile,delimiter=',')
	isFile = True

headers = []
formatString = "=" # The equal sign in the format string eliminate C-struct padding

if __name__ == '__main__':
	signal.signal(signal.SIGINT, sigint_handler)

	import pprint

	with open(options.formatFile) as format_file:
		formats = json.load(format_file)
		diag = formats["formats"]["6"]["payload"]
		#shared = formats["shared"]

	for item in diag:
		#pprint.pprint(str(item))
		#if str(item) in shared:
		#	pprint.pprint(shared[item])
		if item == 'version':
			headers.append(item)
			formatString += 'B'
		if item == 'format':
			headers.append(item)
			formatString += 'B'
		if item == 'checksum':
			headers.append(item)
			formatString += 'H'
		if type(item) is dict:
			headers.append(item["name"])
			if item["type"] == 'enum':
				formatString += ('B')
			if item["type"] == 'bitmap':
				formatString += ('B')
			if item["type"] == 'int8_t':
				formatString += ('b')
			if item["type"] == 'uint8_t':
				formatString += ('B')
			if item["type"] == 'int16_t':
				formatString += ('h')
			if item["type"] == 'uint16_t':
				formatString += ('H')
			if item["type"] == 'int32_t':
				formatString += ('i')	
			if item["type"] == 'uint32_t':
				formatString += ('I')
			if item["type"] == 'float':
				formatString += ('f')

	if isFile:
		csvwriter.writerow(headers)

	print headers
	print formatString

	sc.openConnection(options.portname,57600)

	while (True):
		length = struct.calcsize(formatString)
		data = sc.read(length)

		if data is not None:
			values = struct.unpack(formatString,data)
			if isFile:
				csvwriter.writerow(values)

			os.system('clear')
			for i in range(len(headers)):
				filler = 30 - len(headers[i])
				print ' '*filler,
				print headers[i] + ": ",
				print values[i]
