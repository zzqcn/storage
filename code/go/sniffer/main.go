package main

import (
	"flag"
	"fmt"
	"log"
	"time"

	"github.com/google/gopacket/pcap"

	"github.com/zzqcn/sniffer/data"
	"github.com/zzqcn/sniffer/reader"
	"github.com/zzqcn/sniffer/server"
)

var listdevs = flag.Bool("l", false, "list all devices")
var iface = flag.String("i", "", "Interface to read packets from")
var fname = flag.String("r", "", "Filename to read from, overrides -i")
var flowTimeout = flag.Int("t", 30, "flow timeout (sec)")
var promisc = flag.Bool("promisc", true, "Set promiscuous mode")

func main() {
	var handle *pcap.Handle
	var st *data.Stat
	var err error

	flag.Parse()

	if *listdevs {
		devs, err := pcap.FindAllDevs()
		if err != nil {
			log.Fatalln("list devices failed: ", err)
		}
		for _, d := range devs {
			fmt.Printf("%s:\n", d.Name)
			for _, ip := range d.Addresses {
				fmt.Println("\t", ip.IP)
			}
		}
		return
	}

	if *fname != "" {
		if handle, err = pcap.OpenOffline(*fname); err != nil {
			log.Fatalln("open pcap file failed: ", err)
		}
		st = data.NewStat(*fname)
	} else if *iface != "" {
		handle, err = pcap.OpenLive(*iface, 65536, true, time.Second)
		if err != nil {
			log.Fatalln("open device failed: ", err)
		}
		defer handle.Close()

		st = data.NewStat(*iface)
	} else {
		flag.Usage()
	}

	go reader.Run(handle, st)
	go data.FlowTableAging(st, time.Duration(*flowTimeout)*time.Second)
	server.Run(st)
}
