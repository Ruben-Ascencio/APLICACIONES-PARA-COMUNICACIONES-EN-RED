import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
public class BroadcastRecibe{
    public static void main(String[] args)throws Exception{
        final InetAddress objIA = InetAddress.getByName("224.0.0.1");
        final MulticastSocket objMS = new MulticastSocket(9797);
        objMS.joinGroup(objIA);
        while(true){
            byte[] buffer = new byte[200];
            DatagramPacket objDP = new DatagramPacket(buffer, buffer.length);
            objMS.receive(objDP);
            String paquete = new String(objDP.getData()).trim();
            System.out.println(String.format("%s", paquete));
            if(paquete.equalsIgnoreCase("SALIR")){break;}
        }
        objMS.leaveGroup(objIA);
        objMS.close();
    }
}