from pyfcm import FCMNotification
 
push_service = FCMNotification(api_key="AAAAeob8FZA:APA91bEX2eZoKwgm-2vjeBr712H1d-FngRplKaA_BOZ6LEQtCdIRPYVf3_G2lzGKNMQzy5OGAyOkoHGfjqwex8XgDRFJwMBiPWeoo8NqSlOOCxrh5LHn_fOs7T66teg5FKwkbdH7fx3G")

# Your api-key can be gotten from:  https://console.firebase.google.com/project/<project-name>/settings/cloudmessaging
 
registration_id ="dk-0vrxeO8U:APA91bERbr9LqcdNUmPMhq4xnjBY1qc7wG-tx0bTj-tRQL2CRgqmwt7vV4lNjCrSNc2xd23ox2liDSKIJVWWMySiuyvty0gNgeIhygF2QYNASTH3NKY9boGurpojWrQCXR5nFJwyHrp6"
message_title = "Uber update90"
#message_body = "Your Uber is here1"

data_message = {
    "title": "Uber update20",
	"message":"Hello there21"
}	
		
result = push_service.notify_single_device(registration_id=registration_id, message_title=message_title,data_message = data_message)
 
print(result)
 