package com.nik.bridge;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.kafka.core.KafkaTemplate;
import org.springframework.kafka.support.KafkaHeaders;
import org.springframework.messaging.Message;
import org.springframework.messaging.MessageHandler;
import org.springframework.messaging.MessagingException;
import org.springframework.util.concurrent.ListenableFuture;

import java.util.concurrent.ExecutionException;

public class MqttMessageHandler implements MessageHandler {

    @Autowired
    private KafkaTemplate<String, String> template;
//    private KafkaTemplate<String, Object> template;

    @Override
    public void handleMessage(Message<?> message) throws MessagingException {
        String kafkaTopic = message.getHeaders().get(KafkaHeaders.TOPIC, String.class);
        ListenableFuture future = template.send(kafkaTopic, "data",(String) message.getPayload());
//        ListenableFuture future = template.send(kafkaTopic, (String) message.getPayload());
        try {
            future.get();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }
        System.out.println(message.getPayload());
    }
}
