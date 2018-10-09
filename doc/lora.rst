# Getting Started

## Overview


The ns3 lora module is a simplistic model of a lora-like network. It
supports LoraWAN Class A end devices and one gateway.

## Installing

To install this module, you should do as follow:
1. Download ns-3 latest version (ns-3-27) at [ns-3 main page](https://www.nsnam.org/ns-3-27).
2. Extract ns-allinone-3.27.tar.bz2 file.
3. Download ns3 lora module, then copy it ./src folder inside ns-allinone-3.27 folder.
4. Run three next statements to build source code:
```
	~/ns-allinone-3.27/ns-3$ ./waf clean
	~/ns-allinone-3.27/ns-3$ ./waf configure --enable-tests --enable-examples
	~/ns-allinone-3.27/ns-3$ ./waf build	
```
## Running the tests and examples

To test ns3 lora module, you run this statement:
```
	~/ns-allinone-3.27/ns-3$ ./test.py -s lora-node -m -t lora-node.txt
```
To run example of ns3 lora module, you run this statement:
```
	~/ns-allinone-3.27/ns-3$ ./waf --run lora-example
```
## License

This software is licensed under the terms of the GNU GPLv2 - see the [LICENSE](LICENSE) file for details.

