package reader

import (
	"log"
	"time"

	"github.com/google/gopacket/layers"

	"github.com/google/gopacket"
	"github.com/google/gopacket/pcap"
	"github.com/zzqcn/sniffer/data"
)

// Run XXX
func Run(handle *pcap.Handle, st *data.Stat) {
	var source *gopacket.PacketSource
	var pkt gopacket.Packet
	var layer gopacket.Layer
	var fk data.FlowKey
	var err error

	source = gopacket.NewPacketSource(handle, handle.LinkType())
	source.Lazy = true
	source.NoCopy = true

	log.Println("reader is running...")

	for {
		pkt, err = source.NextPacket()
		if err != nil {
			log.Println("read packet error: ", err)
			continue
		}
		if pkt == nil {
			continue
		}

		log.Printf("recv %d packets\n", st.ReadPacketCnt)
		st.Lock.Lock()
		st.ReadPacketCnt++
		st.Lock.Unlock()

		if pkt.NetworkLayer() == nil {
			continue
		}
		if pkt.TransportLayer() == nil {
			continue
		}

		l3type := pkt.NetworkLayer().LayerType()
		if l3type != layers.LayerTypeIPv4 && l3type != layers.LayerTypeIPv6 {
			continue
		}
		l3flow := pkt.NetworkLayer().NetworkFlow()
		l4type := pkt.TransportLayer().LayerType()
		if l4type != layers.LayerTypeTCP && l4type != layers.LayerTypeUDP {
			continue
		}
		l4flow := pkt.TransportLayer().TransportFlow()

		if l4type == layers.LayerTypeTCP {
			st.Lock.Lock()
			st.TCPPacketCnt++
			st.Lock.Unlock()
			layer = pkt.Layer(layers.LayerTypeTCP)
			tcp, _ := layer.(*layers.TCP)
			if tcp.SrcPort == 80 || tcp.DstPort == 80 {
				st.Lock.Lock()
				st.HTTPPacketCnt++
				st.Lock.Unlock()
			}
		} else if l4type == layers.LayerTypeUDP {
			st.Lock.Lock()
			st.UDPPacketCnt++
			st.Lock.Unlock()
		}

		fk.L3Hash = l3flow.FastHash()
		fk.L4Hash = l4flow.FastHash()
		fk.L4Type = l4type
		st.Lock.Lock()
		fv, ok := st.Table[fk]
		if !ok {
			fv.L3Flow = l3flow
			fv.L4Flow = l4flow
			fv.PacketCnt = 1
			st.CurrentFlowCnt++
		} else {
			fv.PacketCnt++
		}
		fv.UpdateTime = time.Now()
		st.Table[fk] = fv
		st.Lock.Unlock()
	}
}
