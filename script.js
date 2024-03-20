document.addEventListener('DOMContentLoaded', () => {
  const connectButton = document.getElementById('connectButton');
  const button33Status = document.getElementById('lightStatus1');
  const button25Status = document.getElementById('lightStatus2');
  const brightnessDisplay1 = document.getElementById('brightness1');
  const brightnessDisplay2 = document.getElementById('brightness2');
  const modeDisplay = document.getElementById('mode');

  let port, reader;
  let ledState1 = 'Off'; // Initial LED state for button 33
  let ledState2 = 'Off'; // Initial LED state for button 25
  let mode = 'Dimming Cool Light'; // Initial mode
  let brightness1 = 0;
  let brightness2 = 0;

  async function connect() {
    try {
      port = await navigator.serial.requestPort();
      await port.open({ baudRate: 9600 });

      document.getElementById('connectButton').style.backgroundColor = 'green';

      const decoder = new TextDecoderStream();
      port.readable.pipeTo(decoder.writable);
      const inputStream = decoder.readable;
      reader = inputStream.getReader();
      readLoop();
    } catch (error) {
      console.error('There was an error opening the serial port:', error);
      document.getElementById('connectButton').style.backgroundColor = 'red';
    }
  }

  async function readLoop() {
    while (true) {
      const { value, done } = await reader.read();
      if (done) {
        console.log("Closing connection");
        reader.releaseLock();
        break;
      }

      if (value.startsWith("PotValue:")) {
        const rawValue = parseInt(value.split(':')[1]);
        const percentageValue = Math.round((rawValue / 255) * 100);
        
        if (mode === 'Dimming Cool Light' && ledState1 === 'On') {
          brightness1 = percentageValue;
          brightnessDisplay1.textContent = 'Brightness: ' + percentageValue + '%';
        } else if (mode === 'Dimming Warm Light' && ledState2 === 'On') {
          brightness2 = percentageValue;
          brightnessDisplay2.textContent = 'Brightness: ' + percentageValue + '%';
        }
      }

      if (value.includes("Cool light on")) {
        const coolLight = document.getElementById('coolLight');
        ledState1 = 'On'; // Toggle state
        button33Status.textContent = 'Cool Light: ' + ledState1;
        coolLight.classList.add('cool-on');
        coolLight.classList.remove('off');
        brightnessDisplay1.textContent = 'Brightness: ' + brightness1 + '%';
      } else if (value.includes("Cool light off")) {
        const coolLight = document.getElementById('coolLight');
        ledState1 = 'Off'; // Toggle state
        button33Status.textContent = 'Cool Light: ' + ledState1;
        coolLight.classList.remove('cool-on');
        coolLight.classList.add('off');
        brightnessDisplay1.textContent = 'Brightness: 0%';
      } 
      else if (value.includes("Warm light on")) {
        const warmLight = document.getElementById('warmLight');
        ledState2 = 'On'; // Toggle state
        button25Status.textContent = 'Warm Light: ' + ledState2;
        warmLight.classList.add('warm-on');
        warmLight.classList.remove('off');
        brightnessDisplay2.textContent = 'Brightness: ' + brightness2 + '%';
      } else if (value.includes("Warm light off")) {
        const warmLight = document.getElementById('warmLight');
        ledState2 = 'Off'; // Toggle state
        button25Status.textContent = 'Warm Light: ' + ledState2;
        warmLight.classList.remove('warm-on');
        warmLight.classList.add('off');
        brightnessDisplay2.textContent = 'Brightness: 0%';
      } 
      else if (value.includes("Dimming cool light")) {
        mode = 'Dimming Cool Light';
        const toggleCircle = document.querySelector('.toggle-circle');  
        toggleCircle.style.left = '0'; // Move circle to the left
        toggleCircle.style.backgroundColor = 'blue';
      } else if (value.includes("Dimming warm light")) {
        mode = 'Dimming Warm Light';
        const toggleCircle = document.querySelector('.toggle-circle');  
        toggleCircle.style.left = '70px'; // Move circle to the left
        toggleCircle.style.backgroundColor = 'red';
      }
    }
  }

  connectButton.addEventListener('click', connect);
});
