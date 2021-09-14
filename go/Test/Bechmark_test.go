package benchmark_test

import(
	"testing"
	"bytes"
)

func TestConcatStringByAdd(t*testing.T)  {
	elems := []string{"1","2","3","4","5"}
	ret := ""
	for _, ele := range elems {
		ret += ele
	}
}

func TestConcatStringByBytesBuffer(t*testing.T)  {
	var buf bytes.Buffer
	elem := []string{"1","2","3","4","5"}
	for _, ele := range elem {
		buf.WriteString(ele)
	}
}
func BenchmarkConcatStringByAdd(b *testing.B) {

	elems := []string{"1", "2", "3", "4", "5"}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ret := ""
		for _, elem := range elems {
			ret += elem
		}
	}
	b.StopTimer()
}

func BenchmarkConcatStringByByteBuffer(b *testing.B)  {
	elems := []string{"1", "2", "3", "4", "5"}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		var buf bytes.Buffer
		for _, elem := range elems {
			buf.WriteString(elem)

		}
	}
	b.StopTimer()
}