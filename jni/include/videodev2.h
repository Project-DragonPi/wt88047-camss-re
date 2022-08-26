#define v4l2_fourcc(a, b, c, d)\
	((__u32)(a) | ((__u32)(b) << 8) | ((__u32)(c) << 16) | ((__u32)(d) << 24))
	
#define V4L2_PIX_FMT_SBGGR10 v4l2_fourcc('B', 'G', '1', '0')
	

