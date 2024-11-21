#pragma once

// https://developer.mozilla.org/ja/docs/Web/HTTP/Status

enum HTTPStatusCode
{
	// 100~ : informational response
	STATUS_100 = 100, // Continue
	STATUS_101 = 101, // Switching Protocols
	STATUS_102 = 102, // Processing
	STATUS_103 = 103, // Early Hints
	// 200~ : success response
	STATUS_200 = 200, // OK
	STATUS_201 = 201, // Created
	STATUS_202 = 202, // Accepted
	STATUS_203 = 203, // Non-Authoritative Information
	STATUS_204 = 204, // No Content
	STATUS_205 = 205, // Reset Content
	STATUS_206 = 206, // Partial Content
	STATUS_207 = 207, // Multi-Status
	STATUS_208 = 208, // Already Reported
	STATUS_226 = 226, // IM Used
	// 300~ : redirection response
	STATUS_300 = 300, // Multiple Choice
	STATUS_301 = 301, // Moved Permanently
	STATUS_302 = 302, // Found
	STATUS_303 = 303, // See Other
	STATUS_304 = 304, // Not Modified
	STATUS_305 = 305, // Use Proxy (not recommended)
	STATUS_306 = 306, // (unused)
	STATUS_307 = 307, // Temporary Redirect
	STATUS_308 = 308, // Permanent Redirect
	// 400~ : client error response
	STATUS_400 = 400, // Bad Request
	STATUS_401 = 401, // Unauthorized
	STATUS_402 = 402, // Payment Required
	STATUS_403 = 403, // Forbidden
	STATUS_404 = 404, // Not Found
	STATUS_405 = 405, // Method Not Allowed
	STATUS_406 = 406, // Not Acceptable
	STATUS_407 = 407, // Proxy Authentication Required
	STATUS_408 = 408, // Request Timeout
	STATUS_409 = 409, // Conflict
	STATUS_410 = 410, // Gone
	STATUS_411 = 411, // Length Required
	STATUS_412 = 412, // Precondition Failed
	STATUS_413 = 413, // Payload Too Large
	STATUS_414 = 414, // URI Too Long
	STATUS_415 = 415, // Unsupported Media Type
	STATUS_416 = 416, // Range Not Satisfiable
	STATUS_417 = 417, // Expectation Failed
	STATUS_418 = 418, // I'm a teapot
	STATUS_421 = 421, // Misdirected Request
	STATUS_422 = 422, // Unprocessable Entity
	STATUS_423 = 423, // Locked
	STATUS_424 = 424, // Failed Dependency
	STATUS_425 = 425, // Too Early
	STATUS_426 = 426, // Upgrade Required
	STATUS_428 = 428, // Precondition Required
	STATUS_429 = 429, // Too Many Requests
	STATUS_431 = 431, // Request Header Fields Too Large
	STATUS_451 = 451, // Unavailable For Legal Reasons
	// 500~ : server error response
	STATUS_500 = 500, // Internal Server Error
	STATUS_501 = 501, // Not Implemented
	STATUS_502 = 502, // Bad Gateway
	STATUS_503 = 503, // Service Unavailable
	STATUS_504 = 504, // Gateway Timeout
	STATUS_505 = 505, // HTTP Version Not Supported
	STATUS_506 = 506, // Variant Also Negotiates
	STATUS_507 = 507, // Insufficient Storage
	STATUS_508 = 508, // Loop Detected
	STATUS_510 = 510, // Not Extended
	STATUS_511 = 511 // Network Authentication Required
};

