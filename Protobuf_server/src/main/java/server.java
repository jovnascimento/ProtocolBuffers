import protocol.Protocolo;

import java.io.*;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;

@SuppressWarnings("InfiniteLoopStatement")

public class server {
    public static void main(String[] args) throws IOException {
        ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
        serverSocketChannel.bind(new InetSocketAddress(InetAddress.getByName("localhost"), 3000));
        while(true){
            try{
                System.out.println("Esperando pelo cliente, porta 3000");
                SocketChannel client = serverSocketChannel.accept();
                ByteBuffer buf = ByteBuffer.allocate(1024);
                int numBytesRead = client.read(buf);
                if (numBytesRead == -1) {
                    client.close();
                    continue;
                }
                buf.flip();

                Protocolo.Mensagem mensagem = Protocolo.Mensagem.parseFrom(buf);
                switch (mensagem.getId()) {
                    case 1 -> {
                        System.out.println("Mensagem: " + mensagem.getMsg());
                        ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
                        Protocolo.Mensagem resposta = Protocolo.Mensagem.newBuilder().setRespMsg("Servidor diz: " + mensagem.getMsg()).build();
                        byteBuffer.put(resposta.toByteArray());
                        System.out.println(resposta);
                        byteBuffer.flip();
                        client.write(byteBuffer);
                    }
                    case 2 -> {
                        String titulo = mensagem.getNome();
                        String conteudo = mensagem.getConteudo();

                        try {
                            FileWriter file = new FileWriter("C:\\Users\\jovna\\Desktop\\" + titulo, true);
                            PrintWriter out = new PrintWriter(file);
                            out.println(conteudo);
                            out.close();
                        } catch (IOException e ) {
                            e.printStackTrace();
                        }

                        ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
                        Protocolo.Mensagem resposta = Protocolo.Mensagem.newBuilder().setNome(titulo).setConteudo(conteudo).build();
                        byteBuffer.put(resposta.toByteArray());
                        System.out.println(resposta);
                        byteBuffer.flip();
                        client.write(byteBuffer);

                    }
                    case 3 -> {
                        int num1 = mensagem.getNum1();
                        int num2 = mensagem.getNum2();
                        int soma = num1 + num2;
                        ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
                        Protocolo.Mensagem resposta = Protocolo.Mensagem.newBuilder().setSoma(soma).build();
                        byteBuffer.put(resposta.toByteArray());
                        System.out.println(resposta);
                        byteBuffer.flip();
                        client.write(byteBuffer);
                    }
                }
                client.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}