package org.letbethelight.povdisplay.controller;

import org.letbethelight.povdisplay.Service.MessageService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@CrossOrigin(origins = "*", maxAge = 3600)
@RestController
@RequestMapping("/api")
public class MessageController {

    private MessageService messageService;


    @Autowired
    public void setMessageService(MessageService messageService){
        this.messageService = messageService;
    }

    @RequestMapping(method = RequestMethod.GET, path = "/{message}")
    public ResponseEntity<?> sendMessage(@PathVariable String message){

        messageService.setMessage(message);

        return new ResponseEntity<>(HttpStatus.ACCEPTED);

    }

    @RequestMapping(method = RequestMethod.GET, path = "/")
    public ResponseEntity<String> sendWord(){

        String word = messageService.getWord();

        return new ResponseEntity<>(word, HttpStatus.ACCEPTED);

    }
}
