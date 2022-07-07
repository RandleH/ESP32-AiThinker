

console.log(window);


var firebaseConfig = {
  apiKey: "AIzaSyANzso7XRzHlsUhhQcOy51oMihIm-43-KY",
  authDomain: "esp32-wifi-monitor.firebaseapp.com",
  databaseURL: "https://esp32-wifi-monitor-default-rtdb.firebaseio.com",
  projectId: "esp32-wifi-monitor",
  storageBucket: "esp32-wifi-monitor.appspot.com",
  messagingSenderId: "215970898603",
  appId: "1:215970898603:web:ee90b4a8ae6f7198694dee",
  measurementId: "G-CV1X78HW76"
};

// Initialize Firebase
firebase.initializeApp(firebaseConfig);



$(document).ready(
    
	function(){
        var database = firebase.database();
        var led1,wifi,ble,airplane;

        database.ref().on("value", function(snap){
    	        led1 = snap.val().led1;
    	        if( led1=="1" ){
          			document.getElementById("tog-off-led" ).style.display = "none";
          			document.getElementById("tog-on-led").style.display = "block";
          			// led1 = "0";
          		}else{
          			document.getElementById("tog-off-led" ).style.display = "block";
          			document.getElementById("tog-on-led").style.display = "none";
          			// led1 = "1";
          		}

              wifi = snap.val().wifi;
              if( wifi=="1" ){
                document.getElementById("tog-off-wifi" ).style.display = "none";
                document.getElementById("tog-on-wifi").style.display = "block";
                // wifi = "0";
              }else{
                document.getElementById("tog-off-wifi" ).style.display = "block";
                document.getElementById("tog-on-wifi").style.display = "none";
                // wifi = "1";
              }

              ble = snap.val().ble;
              if( ble=="1" ){
                document.getElementById("tog-off-BLE" ).style.display = "none";
                document.getElementById("tog-on-BLE").style.display = "block";
                // ble = "0";
              }else{
                document.getElementById("tog-off-BLE" ).style.display = "block";
                document.getElementById("tog-on-BLE").style.display = "none";
                // ble = "1";
              }
              
              // console.log(`database before ${airplane}`);
              airplane = snap.val().airplane;
              // console.log(`database ${airplane}`);
              if( airplane=="1" ){
                document.getElementById("tog-off-Airplane" ).style.display = "none";
                document.getElementById("tog-on-Airplane").style.display = "block";
                // airplane = "0";
              }else{
                document.getElementById("tog-off-Airplane" ).style.display = "block";
                document.getElementById("tog-on-Airplane").style.display = "none";
                // airplane = "1";
              }
              // console.log(`now ${airplane}`);
            }
        );

        [document.getElementById("tog-off-led"),document.getElementById("tog-on-led")].forEach( item=> {
            item.addEventListener('click', event=>{
              var firebaseRef = firebase.database().ref().child("led1");
              if( led1=="1"){
                firebaseRef.set("0");
                led1 = "0";
              }else{
                firebaseRef.set("1");
                led1 = "1";
              }
            })
        });

        [document.getElementById("tog-off-wifi"),document.getElementById("tog-on-wifi")].forEach( item=> {
            item.addEventListener('click', event=>{
              var firebaseRef = firebase.database().ref().child("wifi");
              if( wifi=="1"){
                firebaseRef.set("0");
                wifi = "0";
              }else{
                firebaseRef.set("1");
                wifi = "1";
              }
            })
        });

        [document.getElementById("tog-off-BLE"),document.getElementById("tog-on-BLE")].forEach( item=> {
            item.addEventListener('click', event=>{
              var firebaseRef = firebase.database().ref().child("ble");
              if( ble=="1"){
                firebaseRef.set("0");
                ble = "0";
              }else{
                firebaseRef.set("1");
                ble = "1";
              }
            })
        });

        [document.getElementById("tog-off-Airplane"),document.getElementById("tog-on-Airplane")].forEach( item=> {
            item.addEventListener('click', event=>{
              console.log(`airplane clicked, ${airplane}`);
              var firebaseRef = firebase.database().ref().child("airplane");
              if( airplane=="1"){
                firebaseRef.set("0");
                airplane = "0";
              }else{
                firebaseRef.set("1");
                airplane = "1";
              }
              console.log(`then ${airplane}`);
            })
        });


        // $(".toggle").click(
        // 	function(){


        // 		var firebaseRef = firebase.database().ref().child("led1");
        // 	    
        // 	}
        // )


	}

)








