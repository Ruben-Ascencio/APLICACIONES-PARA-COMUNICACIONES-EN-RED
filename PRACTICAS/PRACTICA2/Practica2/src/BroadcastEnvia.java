import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Scanner;
public class BroadcastEnvia{
    public static void main(String[] args) throws Exception{
        final InetAddress objIA = InetAddress.getByName("224.0.0.1");
        final DatagramSocket objDS = new DatagramSocket();
        Scanner teclado = new Scanner(System.in);
        while(true){
            String enviar = teclado.nextLine();
            byte[] buffer = enviar.getBytes();
            DatagramPacket objDP = new DatagramPacket(buffer, buffer.length, objIA, 9797);
            objDS.send(objDP);
            if(enviar.equalsIgnoreCase("SALIR")){break;}
        }
        teclado.close();
        objDS.close();
    }
}