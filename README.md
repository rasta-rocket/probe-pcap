# Probe Pcap exercise

## Install
    $ make probe-pcap

Tested on Ubuntu 14.04.

## Usage
    $ sudo ./probe-pcap -i wlan0 dst_port=53
    Device choosen : wlan0
    Filter expression :  dst port 53
    Here is the captured packets:
    ��'���L4��
    Here is the captured packets:
    ��'���L4��
    Here is the captured packets:
    ��'���L4��
    Here is the captured packets:
    ��'���L4��

You must specify the interface on which you listen packets.
Regarding the filters they are optional. The filters implemented are the basic one that you can found at [pcap filters](http://www.tcpdump.org/manpages/pcap-filter.7.html) and for filters which are two-words composed you must put a "_" between them on the command line.

## Help
    Usage: probe-pcap [OPTION...] [FILTERS...]
    pcap exercise

        -i, --interfaces=<name>    interface name
        -?, --help                 Give this help list
        --usage                    Give a short usage message
        -V, --version              Print program version

    Mandatory or optional arguments to long options are also mandatory or optional
    for any corresponding short options.

    Report bugs to <bruno.fernandos@yahoo.com>.

## Version
  v.1.0.0
  
## Prerequisites
 * [libpcap](http://www.tcpdump.org/#latest-releases)
