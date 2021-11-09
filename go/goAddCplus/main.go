package main

/*

#cgo CFLAGS: -Iinclude

#cgo LDFLAGS: -Llib -llibvideo

#include "video.h"

*/
import "C"

import "fmt"

func main() {
    cmd := C.CString("ffmpeg -i ./xxx/*.png ./xxx/yyy.mp4")
    C.exeFFmpegCmd(&cmd)
}