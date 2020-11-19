package server

import (
	"fmt"
	"log"
	"net/http"

	"github.com/zzqcn/sniffer/data"
)

var _stat *data.Stat

func handler(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "text/html; charset=utf-8")

	_stat.Lock.Lock()
	fmt.Fprintf(w, "<p>device: %s<br/>", _stat.NICName)
	fmt.Fprintf(w, "recv: %d<br/>", _stat.ReadPacketCnt)
	fmt.Fprintf(w, "tcp:  %d<br/>", _stat.TCPPacketCnt)
	fmt.Fprintf(w, "udp:  %d<br/>", _stat.UDPPacketCnt)
	fmt.Fprintf(w, "http: %d<br/>", _stat.HTTPPacketCnt)
	fmt.Fprintf(w, "current flow: %d<br/>", _stat.CurrentFlowCnt)
	fmt.Fprintf(w, "timeout flow: %d</p>", _stat.TimeoutFlowCnt)

	fmt.Fprintf(w, "<p>flow table:</p><p><table border>")
	for k, v := range _stat.Table {
		fmt.Fprintf(w, "<tr>")
		fmt.Fprintf(w, "<td>%v</td>", v.L3Flow.Src().String())
		fmt.Fprintf(w, "<td>%v</td>", v.L3Flow.Dst().String())
		fmt.Fprintf(w, "<td>%v</td>", k.L4Type.String())
		fmt.Fprintf(w, "<td>%v</td>", v.L4Flow.Src().String())
		fmt.Fprintf(w, "<td>%v</td>", v.L4Flow.Dst().String())
		fmt.Fprintf(w, "<td>%v</td>", v.PacketCnt)
		fmt.Fprintf(w, "<td>%v</td>", v.UpdateTime.Format("2006-01-02 15:04:05"))
		fmt.Fprintf(w, "</tr>")
	}
	fmt.Fprintf(w, "</table></p>")
	_stat.Lock.Unlock()
}

// Run XXX
func Run(stat *data.Stat) {
	_stat = stat
	http.HandleFunc("/", handler)
	log.Fatal(http.ListenAndServe("localhost:8000", nil))
}
