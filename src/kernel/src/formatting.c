#include <formatting.h>



// Helper: write a character to buffer
static inline void _put_char(char* buf, u32* idx, char c) {
    buf[(*idx)++] = c;
}

// Helper: convert unsigned integer to string (any base)
static inline void _uint_to_str(char* buf, u32* idx, u64 num, u32 base) {
    char digits[64];
    const char* charset = "0123456789ABCDEF";
    i32 i = 0;


    if (num == 0) {
        _put_char(buf, idx, '0');
        return;
    }
    
    while (num > 0) {
        digits[i++] = charset[num % base];
        num /= base;
    }
    
    // Write in reverse order
    while (--i >= 0) {
        _put_char(buf, idx, digits[i]);
    }
}

// Helper: convert signed integer to string
static inline void _int_to_str(char* buf, u32* idx, i64 num) {
    if (num < 0) {
        _put_char(buf, idx, '-');
        num = -num;
    }
    _uint_to_str(buf, idx, (u64)num, 10);
}

// Helper: write a string to buffer
static inline void _put_str(char* buf, u32* idx, const char* str) {
    if (!str) {
        str = "(null)";
    }
    while (*str) {
        _put_char(buf, idx, *str++);
    }
}

static inline void _float_to_str(char* buf, u32* idx, f64 num, u32 precision) { // TODO implement fpu first
    // Handle negative
    if (num < 0) {
        _put_char(buf, idx, '-');
        num = -num;
    }
    
    // Get integer part
    u64 int_part = (u64)num;
    f64 frac_part = num - (f64)int_part;
    
    // Print integer part (simple method)
    char digits[32];
    i32 i = 0;
    
    if (int_part == 0) {
        _put_char(buf, idx, '0');
    } else {
        while (int_part > 0) {
            digits[i++] = '0' + (int_part % 10);
            int_part /= 10;
        }
        while (--i >= 0) {
            _put_char(buf, idx, digits[i]);
        }
    }
    
    // Print decimal point and fractional part
    if (precision > 0) {
        _put_char(buf, idx, '.');
        
        for (u32 p = 0; p < precision; p++) {
            frac_part *= 10;
            u32 digit = (u32)frac_part;
            _put_char(buf, idx, '0' + digit);
            frac_part -= digit;
        }
    }
}

// Format string into buffer with va_list (like vsprintf)
// Supports: %d %i (signed int), %u (unsigned), %x %X (hex), %c (char), %s (string), %p (pointer), %% (literal %)
void vformat_str(char* buf, const char* fmt, va_list args) {
    u32 idx = 0;
    
    while (*fmt != '\0') {
        if (*fmt == '%') {
            fmt++;
            
            switch (*fmt) {
                case 'i': {
                    i64 num = va_arg(args, i32);
                    _int_to_str(buf, &idx, num);
                    break;
                }
                case 'u': {
                    u64 num = va_arg(args, u32);
                    _uint_to_str(buf, &idx, num, 10);
                    break;
                }
                case 'x': {
                    u64 num = va_arg(args, u32);
                    _put_char(buf, &idx, '0');
                    _put_char(buf, &idx, 'x');
                    _uint_to_str(buf, &idx, num, 16);
                    break;
                }
                case 'p': {
                    void* ptr = va_arg(args, void*);
                    _put_str(buf, &idx, "0x");
                    _uint_to_str(buf, &idx, (u64)ptr, 16);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    _put_char(buf, &idx, c);
                    break;
                }
                case 's': {
                    const char* str = va_arg(args, const char*);
                    _put_str(buf, &idx, str);
                    break;
                }
                case 'f': {
                    f64 f = va_arg(args, f64);  // float promotes to double in varargs
                    _float_to_str(buf, &idx, f, 6);  // 6 decimal places default
                    break;
                }
                default: {
                    // Unknown specifier, just print it
                    _put_char(buf, &idx, '%');
                    _put_char(buf, &idx, *fmt);
                    break;
                }
            }
        } else {
            _put_char(buf, &idx, *fmt);
        }
        fmt++;
    }
    
    // Null-terminate
    buf[idx] = '\0';
}

// Wrapper that takes variadic arguments
void format_str(char* buf, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vformat_str(buf, fmt, args);
    va_end(args);
}