#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H

#define MAKE_SINGLE_CASE(class_name )\
public:\
    static class_name& get()\
    {\
        static class_name static_instance_##class_name;\
        return static_instance_##class_name;\
    }\
public:\
    class_name(const class_name& ) = delete;\
    class_name(const class_name&&) = delete;\
    class_name& operator =( const class_name& ) = delete;\
    class_name& operator =( const class_name&& ) = delete;


#define CUDACHECK(cmd) do {                         \
  cudaError_t e = cmd;                              \
  if( e != cudaSuccess ) {                          \
    printf("Failed: Cuda error %s:%d '%s'\n",             \
        __FILE__,__LINE__,cudaGetErrorString(e));   \
    exit(EXIT_FAILURE);                             \
  }                                                 \
} while(0)


#define NCCLCHECK(cmd) do {                         \
  ncclResult_t r = cmd;                             \
  if (r != ncclSuccess) {                            \
    printf("Failed, NCCL error %s:%d '%s'\n",             \
        __FILE__,__LINE__,ncclGetErrorString(r));   \
    exit(EXIT_FAILURE);                             \
  }                                                 \
} while(0)

#endif // COMMON_MACRO_H
