import java.io.*;
import java.net.*;
import java.time.*;
public class BroadcastRecibe{
    public static void main(String[] args)throws Exception{
        final InetAddress objIA = InetAddress.getByName("224.0.0.1");
        final MulticastSocket objMS = new MulticastSocket(9797);
        LocalDateTime objLDT = LocalDateTime.now();
        String ruta = "Documentos/" + objLDT.getHour() + "_" + objLDT.getMinute() + "_" + objLDT.getSecond();
        File objCarpeta = new File(ruta);
        if(!objCarpeta.exists()){
            if(objCarpeta.mkdirs()){System.out.println("Carpeta creada\n" + ruta);}
            else{System.out.println("No se creo la carpeta");}
        }
        else{System.out.println("La carpeta ya existe");}
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