#ifndef __LIBMAPSERVER_BRIDGE
#define __LIBMAPSERVER_BRIDGE

#ifdef __cplusplus
extern "C"  {
class mapObj;
#endif

mapObj * umnms_new_map(char *filename);

#ifdef __cplusplus
}
#endif

#endif // __LIBMAPSERVER_BRIDGE
