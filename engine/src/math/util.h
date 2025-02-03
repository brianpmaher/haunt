#pragma once

#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) < (b) ? (b) : (a))
#define abs(a) ((a) > 0 ? (a) : -(a))
#define mod(a, m) (((a) % (m)) >= 0 ? ((a) % (m)) : (((a) % (m)) + (m)))
#define square(x) ((x) * (x))
