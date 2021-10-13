package map_test

import(
	"testing"
)

func TestInitmap(t*testing.T)  {
	m1 := map[int]int{1:1,2:4,3:9}
	t.Log(m1[2])
	t.Logf("len %d",len(m1))
	m2 := map[int]int{}
	m2[4] = 16
	t.Logf("len %d",len(m2))
	m3 := make(map[int]int, 10)
	t.Logf("len %d",len(m3))
}