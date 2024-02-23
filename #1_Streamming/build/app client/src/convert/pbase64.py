
import base64
  
sample_string = '''
	duy-send-something-data-for-lcd-oke?
'''

def tie_b64_enc():
	sample_string_bytes = sample_string.encode("utf-8")
	sample_string.encode()
	
	base64_bytes = base64.b64encode(sample_string_bytes)
	base64_string = base64_bytes.decode("utf-8")
	
	print(f"Encoded string: {base64_string}")
	print(f"data: {len(sample_string)} - enc: {len(base64_string)}")
    
	return base64_string

def tie_b64_enc_from_str_utf8(data_str):
	sample_string_bytes = data_str.encode("utf-8")
	
	base64_bytes = base64.b64encode(sample_string_bytes)
	base64_string = base64_bytes.decode("utf-8")
	
	print(f"Encoded string: {base64_string}")
	print(f"data: {len(data_str)} - enc: {len(base64_string)}")
    
	return base64_string

def tie_b64_enc_from_str_ascii(data_str):
	sample_string_bytes = data_str.encode("ascii")
	
	base64_bytes = base64.b64encode(sample_string_bytes)
	base64_string = base64_bytes.decode("ascii")
	
	print(f"Encoded string: {base64_string}")
	print(f"data: {len(data_str)} - enc: {len(base64_string)}")
    
	return base64_string

def tie_test_b64(data_str):
	encoded = base64.b64encode(data_str)
	# decoded = base64.b64decode(encoded)
	
	# Print original data
	# print(f"Original data : {len(data_str)}")
	# print(f"Encoded data : {len(encoded)}")
	# # Print encoded data
	# print("Encoded data  : {}".format(encoded))
	# # Print decoded data
	# print("Decoded data  : {}".format(decoded))

	return encoded.decode()
