#ifndef __RESULT_H__
#define __RESULT_H__

typedef enum Result_e
{
    resOk = 0,
    resError,
    resInvalidParameter,
    resNotImplemented,
    resInvalidAddress,
    resBufferTooSmall,
    resNoMore,
    resFileNotExit,
    resFileExit,
    resNoMemory,
    resNotMatch,
    resFurtherMore,
    resSymbolUndefined,
} Result;

#ifndef SUCCESS
#define SUCCESS(_result_) ((_result_) == resOk)
#endif
#define FAILED(_result_) ((_result_) != resOk)

#endif /* __RESULT_H__ */