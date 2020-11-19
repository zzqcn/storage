package data

import (
	"log"
	"sync"
	"time"

	"github.com/google/gopacket"
)

// FlowKey XXX
type FlowKey struct {
	L3Hash uint64
	L4Hash uint64
	L4Type gopacket.LayerType
}

// FlowValue XXX
type FlowValue struct {
	L3Flow     gopacket.Flow
	L4Flow     gopacket.Flow
	PacketCnt  uint64
	UpdateTime time.Time
}

// FlowTable XXX
type FlowTable map[FlowKey]FlowValue

// Stat 统计项
type Stat struct {
	NICName        string
	ReadPacketCnt  uint64
	TCPPacketCnt   uint64
	UDPPacketCnt   uint64
	HTTPPacketCnt  uint64
	Table          FlowTable
	CurrentFlowCnt uint32
	TimeoutFlowCnt uint32
	Lock           sync.Mutex
}

// NewStat XXX
func NewStat(name string) *Stat {
	item := new(Stat)
	item = &Stat{
		name, 0, 0, 0, 0,
		map[FlowKey]FlowValue{},
		0, 0,
		sync.Mutex{}}

	return item
}

// FlowTableAging XXX
func FlowTableAging(st *Stat, timeout time.Duration) {
	for {
		select {
		case <-time.After(time.Second * 5):
			st.Lock.Lock()
			for k, v := range st.Table {
				if v.UpdateTime.Add(timeout).Before(time.Now()) {
					delete(st.Table, k)
					st.TimeoutFlowCnt++
					st.CurrentFlowCnt--
					log.Printf("flow %v timeout\n", v.L4Flow)
				}
			}
			st.Lock.Unlock()
		}
	}
}
