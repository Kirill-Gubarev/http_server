let clickCount = 0;

const clickButton = document.getElementById('clickButton');
const clickCountDisplay = document.getElementById('clickCount');

function updateClickCount() {
    clickCount++;
    clickCountDisplay.textContent = `Clicks: ${clickCount}`;
}

clickButton.addEventListener('click', updateClickCount);