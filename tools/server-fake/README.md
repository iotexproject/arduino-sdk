# Fake HTTP-GRPC server

Returns canned responses based on a pre-configured request-response mapping defined in `config.json`.  
If no request match is configured for the request, then a 400 error is returned.  

## Configuration

### Port
Configure the `port` property to the server listening port

### Request-response match
The server can be configured to return a canned request given a request path and a request body.  
In order to add one, add a new element to the `pathMapping` array of the desired `requestPath`.  
The new pathMapping must contain the following properties:
- **description**: no used by the code. A description of what the maping does.
- **requestBody**: the body of the request to match.
- **grpcStatus**: the grpc status to return. Must be a `GrpcStatus` enum.
- **grpcMessage**: the grpc message to return.
- **responseBody**: the response body to return.
- **responseCode**: the response code to return.

## Starting the server
Run the `server_fake.py` script or use the provided VSCode task in the root directory. 