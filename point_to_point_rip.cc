#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("point_to_point_rip");

void TearDownLink (Ptr<Node> node, uint32_t interface)
{
  node->GetObject<Ipv4> ()->SetDown (interface);
}

void RecoverLink (Ptr<Node> node, uint32_t interface)
{
  node->GetObject<Ipv4> ()->SetUp (interface);
}

int main (int argc, char *argv[])
{
  // set time resolution of simulation
  Time::SetResolution (Time::NS);

  // The below value configures the default behavior of global routing.
  // By default, it is disabled.  To respond to interface events, set to true
  // Config::SetDefault ("ns3::Ipv4GlobalRouting::RespondToInterfaceEvents", BooleanValue (true));

  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  // define Internet helpers
  InternetStackHelper internetStackHelper;
  Ipv4StaticRoutingHelper ipv4StaticRoutingHelper;
  Ipv4AddressHelper ipv4AddressHelper;
  
  // Compose internetwork nodes

  // define client node: T
  NodeContainer clientNode_T;
  clientNode_T.Create(1);
  // define router node: A
  NodeContainer routerNode_A;
  routerNode_A.Create(1);
  // define router node: B
  NodeContainer routerNode_B;
  routerNode_B.Create(1);
  // define router node: C
  NodeContainer routerNode_C;
  routerNode_C.Create(1);
  // define server node: R
  NodeContainer serverNode_R;
  serverNode_R.Create(1);

  // Compose internetwork node mobility
  MobilityHelper mobility_clientNode_T;
  Ptr<ListPositionAllocator> positionAlloc_clientNode_T = CreateObject<ListPositionAllocator> ();
  positionAlloc_clientNode_T->Add ( Vector(0.0, 0.0, 0.0));
  mobility_clientNode_T.SetPositionAllocator (positionAlloc_clientNode_T);
  mobility_clientNode_T.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility_clientNode_T.Install (clientNode_T);

  MobilityHelper mobility_routerNode_A;
  Ptr<ListPositionAllocator> positionAlloc_routerNode_A = CreateObject<ListPositionAllocator> ();
  positionAlloc_routerNode_A->Add ( Vector(5.0, 0.0, 0.0));
  mobility_routerNode_A.SetPositionAllocator (positionAlloc_routerNode_A);
  mobility_routerNode_A.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility_routerNode_A.Install (routerNode_A);

  MobilityHelper mobility_routerNode_B;
  Ptr<ListPositionAllocator> positionAlloc_routerNode_B = CreateObject<ListPositionAllocator> ();
  positionAlloc_routerNode_B->Add ( Vector(10.0, 0.0, 0.0));
  mobility_routerNode_B.SetPositionAllocator (positionAlloc_routerNode_B);
  mobility_routerNode_B.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility_routerNode_B.Install (routerNode_B);

  MobilityHelper mobility_routerNode_C;
  Ptr<ListPositionAllocator> positionAlloc_routerNode_C = CreateObject<ListPositionAllocator> ();
  positionAlloc_routerNode_C->Add ( Vector(15.0, 0.0, 0.0));
  mobility_routerNode_C.SetPositionAllocator (positionAlloc_routerNode_C);
  mobility_routerNode_C.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility_routerNode_C.Install (routerNode_C);

  MobilityHelper mobility_serverNode_R;
  Ptr<ListPositionAllocator> positionAlloc_serverNode_R = CreateObject<ListPositionAllocator> ();
  positionAlloc_serverNode_R->Add ( Vector(20.0, 0.0, 0.0));
  mobility_serverNode_R.SetPositionAllocator (positionAlloc_serverNode_R);
  mobility_serverNode_R.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility_serverNode_R.Install (serverNode_R);

  RipHelper ripRouting;
  Ipv4ListRoutingHelper list;
  list.Add (ripRouting, 0);

  internetStackHelper.SetRoutingHelper (list);

  // Compose internetworking
  // Add IP protocol stack to network nodes
  internetStackHelper.Install (clientNode_T);
  internetStackHelper.Install (routerNode_A);
  internetStackHelper.Install (routerNode_B);
  internetStackHelper.Install (routerNode_C);
  internetStackHelper.Install (serverNode_R);

  // Compose bidirectional links
  // define bidirectional link "link_T_A"
  PointToPointHelper p2pHelper_link_T_A;
  p2pHelper_link_T_A.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Mb/s")));
  p2pHelper_link_T_A.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (1)));
  NetDeviceContainer netDevices_link_T_A;
  netDevices_link_T_A = p2pHelper_link_T_A.Install (clientNode_T.Get (0), routerNode_A.Get (0));
  // define bidirectional link "link_A_B"
  PointToPointHelper p2pHelper_link_A_B;
  p2pHelper_link_A_B.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Mb/s")));
  p2pHelper_link_A_B.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (1)));
  NetDeviceContainer netDevices_link_A_B;
  netDevices_link_A_B = p2pHelper_link_A_B.Install (routerNode_A.Get (0), routerNode_B.Get (0));
  // define bidirectional link "link_B_C"
  PointToPointHelper p2pHelper_link_B_C;
  p2pHelper_link_B_C.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Mb/s")));
  p2pHelper_link_B_C.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (1)));
  NetDeviceContainer netDevices_link_B_C;
  netDevices_link_B_C = p2pHelper_link_B_C.Install (routerNode_B.Get (0), routerNode_C.Get (0));
  // define bidirectional link "link_C_R"
  PointToPointHelper p2pHelper_link_C_R;
  p2pHelper_link_C_R.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Mb/s")));
  p2pHelper_link_C_R.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (1)));
  NetDeviceContainer netDevices_link_C_R;
  netDevices_link_C_R = p2pHelper_link_C_R.Install (routerNode_C.Get (0), serverNode_R.Get (0));

  // assign IP addresses for bidirectional links
  // assign IP addresses for bidirectional link "link_T_A"
  ipv4AddressHelper.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer ipv4Interface_link_T_A;
  ipv4Interface_link_T_A = ipv4AddressHelper.Assign (netDevices_link_T_A);
  // assign IP addresses for bidirectional link "link_A_B"
  ipv4AddressHelper.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer ipv4Interface_link_A_B;
  ipv4Interface_link_A_B = ipv4AddressHelper.Assign (netDevices_link_A_B);
  // assign IP addresses for bidirectional link "link_B_C"
  ipv4AddressHelper.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer ipv4Interface_link_B_C;
  ipv4Interface_link_B_C = ipv4AddressHelper.Assign (netDevices_link_B_C);
  // assign IP addresses for bidirectional link "link_C_R"
  ipv4AddressHelper.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer ipv4Interface_link_C_R;
  ipv4Interface_link_C_R = ipv4AddressHelper.Assign (netDevices_link_C_R);

  // assign and populate global centralized routing "God" tables
  // Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // define UDP Echo application named "appLink_T_R"
  uint16_t port_appLink_T_R = 9;
  Ipv4Address remoteIpv4Address_appLink_T_R;
  remoteIpv4Address_appLink_T_R = serverNode_R.Get (0)->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
  UdpEchoClientHelper udpechoClient_appLink_T_R (remoteIpv4Address_appLink_T_R, port_appLink_T_R);
  udpechoClient_appLink_T_R.SetAttribute( "MaxPackets", UintegerValue (100));
  udpechoClient_appLink_T_R.SetAttribute( "Interval", TimeValue ( Seconds(1.0)));
  udpechoClient_appLink_T_R.SetAttribute( "PacketSize", UintegerValue (100));
  ApplicationContainer udpechoClientApp_appLink_T_R = udpechoClient_appLink_T_R.Install (clientNode_T.Get (0));
  udpechoClientApp_appLink_T_R.Start (Seconds (1.000));
  udpechoClientApp_appLink_T_R.Stop (Seconds (100));
  // add echo server for echo client
  UdpEchoServerHelper udpechoServer_appLink_T_R (port_appLink_T_R);
  ApplicationContainer udpechoServerApp_appLink_T_R = udpechoServer_appLink_T_R.Install (serverNode_R.Get (0));
  udpechoServerApp_appLink_T_R.Start (Seconds (1.000));
  udpechoServerApp_appLink_T_R.Stop (Seconds (100));

  // Create the animation object and configure for specified output
  AnimationInterface anim ("point_to_point_rip.xml");
  anim.EnablePacketMetadata();

  Simulator::Schedule (Seconds (60), &TearDownLink, routerNode_A.Get(0), 1);
  Simulator::Schedule (Seconds (80), &RecoverLink, routerNode_A.Get(0), 1);

  Simulator::Stop (Seconds (100));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}