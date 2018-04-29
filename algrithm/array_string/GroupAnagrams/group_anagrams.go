package main

import (
	"fmt"
)

func stringSort(s string) string {
	m := make(map[byte]int)

	for i := byte('a'); i <= byte('z'); i++ {
		m[i] = 0
	}

	for i := 0; i < len(s); i++ {
		m[s[i]]++
	}

	res := make([]byte, 0)
	for i := byte('a'); i <= byte('z'); i++ {
		if m[i] > 0 {
			for j := 0; j < m[i]; j++ {
				res = append(res, i)
			}
		}
	}

	return string(res)
}

func impl1(words []string) {
	r := make(map[string][]string)

	for _, w := range words {
		ws := stringSort(w)
		if l, ok := r[ws]; ok {
			r[ws] = append(l, w)
		} else {
			tl := make([]string, 1)
			tl[0] = w
			r[ws] = tl
		}
	}

	for _, list := range r {
		fmt.Println(list)
	}
}

func main() {
	cands := []string{"eat", "tea", "tan", "ate", "nat", "bat"}

	impl1(cands)
}
