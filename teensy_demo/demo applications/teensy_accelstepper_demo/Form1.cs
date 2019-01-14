using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace teensy_serial_demo
{
    public partial class Form1 : Form
    {
        private int receivedPacketCount = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // attempt to open the serial port
            if (!serialPort1.IsOpen)
            {
                serialPort1.PortName = textBox1.Text;
                serialPort1.BaudRate = (int)numericUpDown1.Value;
                try
                {
                    serialPort1.Open();
                    serialPort1.DataReceived += new SerialDataReceivedEventHandler(port_DataReceived);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Exception while opening serial port: " + ex.Message);
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            // attempt to close the serial port
            if (serialPort1.IsOpen)
            {
                serialPort1.Close();
                serialPort1.DataReceived -= port_DataReceived;
            }
        }

        private void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            // read the available bytes
            int numBytes = serialPort1.BytesToRead;
            byte[] buffer = new byte[numBytes];
            serialPort1.Read(buffer, 0, numBytes);

            // convert the bytes to string
            string bufferStr = "";
            for (int i = 0; i < buffer.Length; i++)
            {
                bufferStr += buffer[i].ToString();
                if (i < buffer.Length - 1)
                {
                    bufferStr += " ";
                }
            }

            // increment the number of received packets
            receivedPacketCount++;

            // display the string and packet count
            textBox4.Invoke(new MethodInvoker(() => textBox4.Text = bufferStr));
            textBox5.Invoke(new MethodInvoker(() => textBox5.Text = receivedPacketCount.ToString()));
        }

        private void button3_Click(object sender, EventArgs e)
        {
            // get the payload string
            string s = textBox3.Text;
            string[] payloadStrings = s.Split(' ');

            // parse individual bytes from the string
            byte[] payloadBuffer;
            int payloadLength = payloadStrings.Length;
            payloadBuffer = new byte[payloadLength];
            for(int i = 0; i < payloadLength; i++)
            {
                payloadBuffer[i] = Convert.ToByte(payloadStrings[i]);
            }

            // create the packet buffer
            int packetLength = payloadBuffer.Length + 3;
            byte[] packetBuffer = new byte[packetLength];
            packetBuffer[0] = 0xAA;
            packetBuffer[1] = Convert.ToByte(packetLength);
            byte checkSum = Convert.ToByte(packetBuffer[0] ^ packetBuffer[1]);
            for (int i = 0; i < payloadLength; i++)
            {
                checkSum = Convert.ToByte(checkSum ^ payloadBuffer[i]);
                packetBuffer[i + 2] = payloadBuffer[i];
            }
            packetBuffer[packetLength - 1] = checkSum;

            // create the packet buffer string
            string s2 = "";
            for (int i = 0; i < packetLength; i++)
            {
                s2 += packetBuffer[i].ToString();
                if (i < packetLength - 1)
                {
                    s2 += " ";
                }
            }

            // display the packet string
            textBox2.Text = s2;
        }

        private void button5_Click(object sender, EventArgs e)
        {
            // get the packet string
            string s = textBox2.Text;
            string[] packetStrings = s.Split(' ');

            // parse individual bytes from the string
            byte[] packetBuffer;
            int packetLength = packetStrings.Length;
            packetBuffer = new byte[packetLength];
            for (int i = 0; i < packetLength; i++)
            {
                packetBuffer[i] = Convert.ToByte(packetStrings[i]);
            }

            // attempt to write the packet to the serial port
            try
            {
                serialPort1.Write(packetBuffer, 0, packetLength);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Exception while writing to serial port: " + ex.Message);
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            // get the stepper motion parameters
            byte stepID = Convert.ToByte(Math.Abs(numericUpDown2.Value));
            byte[] stepAcceleration = BitConverter.GetBytes(Convert.ToInt32(numericUpDown3.Value));
            byte[] stepVelocity = BitConverter.GetBytes(Convert.ToInt32(numericUpDown4.Value));
            byte[] stepPosition = BitConverter.GetBytes(Convert.ToInt32(numericUpDown5.Value));

            // create the packet payload (MSB first for multiple byte values)
            const byte stepperPacketDescriptor = 0x0A;
            byte[] payloadBuffer = { stepperPacketDescriptor, stepID, 
                                       stepAcceleration[3], stepAcceleration[2], stepAcceleration[1], stepAcceleration[0], 
                                       stepVelocity[3], stepVelocity[2], stepVelocity[1], stepVelocity[0], 
                                       stepPosition[3], stepPosition[2], stepPosition[1], stepPosition[0] };
            int payloadLength = 14;

            // create the packet buffer
            int packetLength = payloadBuffer.Length + 3;
            byte[] packetBuffer = new byte[packetLength];
            packetBuffer[0] = 0xAA;
            packetBuffer[1] = Convert.ToByte(packetLength);
            byte checkSum = Convert.ToByte(packetBuffer[0] ^ packetBuffer[1]);
            for (int i = 0; i < payloadLength; i++)
            {
                checkSum = Convert.ToByte(checkSum ^ payloadBuffer[i]);
                packetBuffer[i + 2] = payloadBuffer[i];
            }
            packetBuffer[packetLength - 1] = checkSum;

            // create the packet buffer string
            string s2 = "";
            for (int i = 0; i < packetLength; i++)
            {
                s2 += packetBuffer[i].ToString();
                if (i < packetLength - 1)
                {
                    s2 += " ";
                }
            }

            // display the packet string
            textBox2.Text = s2;
        }

        private void button6_Click(object sender, EventArgs e)
        {
            // get the stepper motion parameters
            byte stepID = Convert.ToByte(Math.Abs(numericUpDown2.Value));
            byte[] stepAcceleration = BitConverter.GetBytes(Convert.ToInt32(numericUpDown3.Value));
            byte[] stepVelocity = BitConverter.GetBytes(Convert.ToInt32(numericUpDown4.Value));
            byte[] stepPosition = BitConverter.GetBytes(Convert.ToInt32(numericUpDown5.Value));

            // create the packet payload (MSB first for multiple byte values)
            const byte stepperPacketDescriptor = 0x0B;
            byte[] payloadBuffer = { stepperPacketDescriptor, stepID, 
                                       stepAcceleration[3], stepAcceleration[2], stepAcceleration[1], stepAcceleration[0], 
                                       stepVelocity[3], stepVelocity[2], stepVelocity[1], stepVelocity[0], 
                                       stepPosition[3], stepPosition[2], stepPosition[1], stepPosition[0] };
            int payloadLength = 14;

            // create the packet buffer
            int packetLength = payloadBuffer.Length + 3;
            byte[] packetBuffer = new byte[packetLength];
            packetBuffer[0] = 0xAA;
            packetBuffer[1] = Convert.ToByte(packetLength);
            byte checkSum = Convert.ToByte(packetBuffer[0] ^ packetBuffer[1]);
            for (int i = 0; i < payloadLength; i++)
            {
                checkSum = Convert.ToByte(checkSum ^ payloadBuffer[i]);
                packetBuffer[i + 2] = payloadBuffer[i];
            }
            packetBuffer[packetLength - 1] = checkSum;

            // create the packet buffer string
            string s2 = "";
            for (int i = 0; i < packetLength; i++)
            {
                s2 += packetBuffer[i].ToString();
                if (i < packetLength - 1)
                {
                    s2 += " ";
                }
            }

            // display the packet string
            textBox2.Text = s2;
        }

        private void button7_Click(object sender, EventArgs e)
        {
            // create the packet payload (MSB first for multiple byte values)
            const byte executeDeferredDescriptor = 0x0C;
            byte[] payloadBuffer = { executeDeferredDescriptor };
            int payloadLength = 1;

            // create the packet buffer
            int packetLength = payloadBuffer.Length + 3;
            byte[] packetBuffer = new byte[packetLength];
            packetBuffer[0] = 0xAA;
            packetBuffer[1] = Convert.ToByte(packetLength);
            byte checkSum = Convert.ToByte(packetBuffer[0] ^ packetBuffer[1]);
            for (int i = 0; i < payloadLength; i++)
            {
                checkSum = Convert.ToByte(checkSum ^ payloadBuffer[i]);
                packetBuffer[i + 2] = payloadBuffer[i];
            }
            packetBuffer[packetLength - 1] = checkSum;

            // create the packet buffer string
            string s2 = "";
            for (int i = 0; i < packetLength; i++)
            {
                s2 += packetBuffer[i].ToString();
                if (i < packetLength - 1)
                {
                    s2 += " ";
                }
            }

            // display the packet string
            textBox2.Text = s2;
        }
    }
}
