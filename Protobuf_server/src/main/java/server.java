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
        // Abre um socket, passando o IP e a porta para o método bind
        ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
        serverSocketChannel.bind(new InetSocketAddress(InetAddress.getByName("localhost"), 3000));
        // Servidor funcionando em um while(true) para que sempre esteja disponível após enviar a mensagem
        while(true){
            try{
                // Aloca memória para o buffer que receberá a mensagem do cliente e trata em caso de erro
                System.out.println("Esperando pelo cliente, porta 3000");
                SocketChannel client = serverSocketChannel.accept();
                ByteBuffer buf = ByteBuffer.allocate(4096);
                int numBytesRead = client.read(buf);
                if (numBytesRead == -1) {
                    client.close();
                    continue;
                }
                //Troca o tipo da mensagem de read para write
                buf.flip();

                // Mensagem recebe o buffer do cliente
                Protocolo.Mensagem mensagem = Protocolo.Mensagem.parseFrom(buf);
                // Switch case para tratamento, ID enviado na mensagem mostra qual operação ser realizada
                switch (mensagem.getId()) {
                    case 1 -> {
                        System.out.println("Mensagem: " + mensagem.getMsg());
                        ByteBuffer byteBuffer = ByteBuffer.allocate(4096);
                        // Utiliza o método newBuilder() para definir os valores a serem enviados pelo servidor
                        Protocolo.Mensagem resposta = Protocolo.Mensagem.newBuilder().
                                setMsg("Servidor diz: " + mensagem.getMsg()).build();
                        // Transforma a mensagem em bytes para ser enviado pelo protobuf
                        byteBuffer.put(resposta.toByteArray());
                        System.out.println(resposta);
                        byteBuffer.flip();
                        // Envia o buffer de volta ao cliente
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

                        ByteBuffer byteBuffer = ByteBuffer.allocate(4096);
                        // Utiliza o método newBuilder() para definir os valores a serem enviados pelo servidor
                        Protocolo.Mensagem resposta = Protocolo.Mensagem.newBuilder().
                                setNome(titulo).setConteudo(conteudo).build();
                        // Transforma a mensagem em bytes para ser enviado pelo protobuf
                        byteBuffer.put(resposta.toByteArray());
                        System.out.println(resposta);
                        byteBuffer.flip();
                        // Envia o buffer de volta ao cliente
                        client.write(byteBuffer);
                    }
                    case 3 -> {
                        int num1 = mensagem.getNum1();
                        int num2 = mensagem.getNum2();
                        int soma = num1 + num2;
                        ByteBuffer byteBuffer = ByteBuffer.allocate(4096);
                        // Utiliza o método newBuilder() para definir os valores a serem enviados pelo servidor
                        Protocolo.Mensagem resposta = Protocolo.Mensagem.newBuilder().
                                setSoma(soma).build();
                        // Transforma a mensagem em bytes para ser enviado pelo protobuf
                        byteBuffer.put(resposta.toByteArray());
                        System.out.println(resposta);
                        byteBuffer.flip();
                        // Envia o buffer de volta ao cliente
                        client.write(byteBuffer);
                    }
                }
                // Fecha o cliente após a comunicação
                client.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}