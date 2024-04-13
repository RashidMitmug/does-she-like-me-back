document.getElementById("intervalForm").addEventListener("submit", function(event) {
    event.preventDefault();
    var seconds = document.getElementById("seconds").value;
    var minutes = document.getElementById("minutes").value;
    var hours = document.getElementById("hours").value;
    var days = document.getElementById("days").value;
    var interval = seconds + "," + minutes + "," + hours + "," + days;
    sendMessage(interval);
  });

  function sendMessage(interval) {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
      if (xhr.readyState == XMLHttpRequest.DONE) {
        if (xhr.status == 200) {
          document.getElementById("message").innerHTML = "Interval set to " + interval;
        } else {
          document.getElementById("message").innerHTML = "Failed to set interval. Please try again.";
        }
      }
    };
    xhr.open("GET", "/setinterval?interval=" + interval, true);
    xhr.send();
  }