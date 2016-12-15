using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Xml;

namespace eHomeClient
{
	/// <summary>
	/// MainWindow.xaml 的交互逻辑
	/// </summary>
	public partial class MainWindow : Window
	{
		[DllImport("eHomeDemo.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
		static extern int Init();
		[DllImport("eHomeDemo.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
		static extern int Fini();

		[DllImport("eHomeDemo.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
		static extern int Connect(string ip, int port);

		[DllImport("eHomeDemo.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
		static extern int SendData(string sendBuff);

		[DllImport("eHomeDemo.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
		static extern int GetMsg(StringBuilder msgRsp);

		string m_ip
		{
			set;
			get;
		}
		int m_port = 0;

		public MainWindow()
		{
			Init();
			InitializeComponent();

			IpAddr.Text = ReadConfig("ip");
			Port.Text = ReadConfig("port");
			m_ip = IpAddr.Text;
			m_port = Int32.Parse(Port.Text);
		}

		string str_path = "./config.xml";

		string ConfigMsg
		{
			get;
			set;
		}


		public string ReadConfig(string cmd)
		{

			if (File.Exists(str_path))
			{
				XmlDocument configXml = new XmlDocument();
				configXml.Load(str_path);
				XmlNodeList nodelist = configXml.SelectNodes("/info");

				if (nodelist != null)
				{
					foreach (XmlNode node in nodelist)
					{
						foreach (XmlNode ChildNode in node)
						{
							if (ChildNode.Name == cmd)
							{
								ConfigMsg = ChildNode.InnerXml;
								return ConfigMsg;
							}
						}
					}
				}
			}
			return null;
		}

		public void SetConfig(string cmd,string buffer)
		{
			if (File.Exists(str_path))
			{
				XmlDocument configXml = new XmlDocument();
				configXml.Load(str_path);
				XmlNodeList nodelist = configXml.SelectNodes("/info");

				if (nodelist != null)
				{
					foreach (XmlNode node in nodelist)
					{
						foreach (XmlNode ChildNode in node)
						{
							if (ChildNode.Name == cmd)
							{
								ChildNode.InnerXml = buffer;
							}
						}
					}
				}
				configXml.Save(str_path);
			}
		}

		private void ConnectBtn_Click(object sender, RoutedEventArgs e)
		{
			m_ip = IpAddr.Text;
			m_port = Int32.Parse(Port.Text);
			
			if (ConnectBtn.Content.ToString().Equals("连接"))
			{
				
				Connect(m_ip, m_port);
				ConnectBtn.Content = "断开";
				Send_btn.IsEnabled = true;
			}

			else if (ConnectBtn.Content.ToString().Equals("断开"))
			{
				Send_btn.IsEnabled = false;
				ConnectBtn.Content = "连接";
			}
		}
		string testMsg = "<?xml version=\"1.0\" encoding=\"UTF - 8\" ?> <PPVSPMessage><Version>4.0</Version><Sequence>1</Sequence><CommandType>REQUEST</CommandType><Method>REGISTER</Method><Command>REGISTER</Command><Params><SerialNumber>DS-AAAAA</SerialNumber><NetUnitType>PU</NetUnitType><DeviceID>test0123</DeviceID><Password></Password><FirmwareVersion>Vx.y.z buildYYMMDD</FirmwareVersion><LocalIP>10.21.130.53</LocalIP><LocalPort>7660</LocalPort><DevType>1</DevType><Manufacture>0</Manufacture><DetectPackage>TRUE</DetectPackage><ReliableTransmission>TRUE</ReliableTransmission><RegisterServer>10.33.30.218</RegisterServer></Params></PPVSPMessage>";

		private void Send_btn_Click(object sender, RoutedEventArgs e)
		{
			byte[] byteArray = System.Text.Encoding.Default.GetBytes(SendBox.Text);
			string sendBuff = "<?xml version=\"1.0\" encoding=\"UTF-8\"?> ";
			string sendMsg = Encoding.ASCII.GetString(byteArray);
			if (sendMsg.IndexOf(sendBuff) >= 0)
			{
				sendBuff = sendMsg;
			}
			else
			{
				sendBuff += sendMsg;
			}
			SendData(sendBuff);

			StringBuilder msgRsp = new StringBuilder(2048);
			System.Threading.Thread.Sleep(100);
			RecvBox.Text = "";
			do
			{
				GetMsg(msgRsp);
				RecvBox.Text += msgRsp.ToString();
			}
			while (msgRsp.ToString().Equals("") == false);
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			Fini();
			SetConfig("ip",m_ip);
			SetConfig("port", m_port.ToString());
		}

		private void Register_Click(object sender, RoutedEventArgs e)
		{
			SendBox.Text = ReadConfig("Register");
		}

		private void UnRegister_Click(object sender, RoutedEventArgs e)
		{

		}

		private void Restart_Click(object sender, RoutedEventArgs e)
		{

		}

		private void ReReg_Click(object sender, RoutedEventArgs e)
		{

		}

		private void Alarm_Click(object sender, RoutedEventArgs e)
		{
			SendBox.Text = ReadConfig("Alarm");
		}
	}
}
