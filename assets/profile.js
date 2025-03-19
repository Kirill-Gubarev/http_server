document.addEventListener("DOMContentLoaded", function () {
    const user = JSON.parse(localStorage.getItem("user"));
    const input_amount = document.getElementById("topup-amount");
    
    input_amount.addEventListener("input", function(event) {
        const max = 10000;
        let value = input_amount.value.replace(/[^0-9]/g, '');
        if (value && parseInt(value, 10) > max) {
            value = max;
        }
        input_amount.value = value;
    });
    
    if (user) {
        document.getElementById("fullname").textContent = user.fullname;
        document.getElementById("email").textContent = user.email;
        document.getElementById("login").textContent = user.login;
        document.getElementById("balance").textContent = `${user.balance} руб`;

        fetch('/get_purchases', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ action: "get_purchases", login: user.login })
        })
        .then(response => response.json())
        .then(purchases => {
            if (purchases.length > 0) {
                const purchasesContainer = document.createElement("div");
                purchasesContainer.className = "purchases-container";

                const title = document.createElement("h3");
                title.textContent = "Покупки";
                purchasesContainer.appendChild(title);

                purchases.forEach(purchase => {
                    const item = document.createElement("div");
                    item.className = "purchase-item";
                    
                    
                    const total = purchase.price * purchase.quantity;
                    
                    item.innerHTML = `
                        <img src="${purchase.image}" alt="${purchase.product}" class="purchase-image">
                        <div class="purchase-details">
                            <p><strong>${purchase.product}</strong></p>
                            <p>Цена: ${purchase.price} руб</p>
                            <p>Количество: ${purchase.quantity}</p>
                            <p>Сумма: ${total} руб</p>
                            <p>Дата: ${purchase.date}</p>
                        </div>
                    `;
                    purchasesContainer.appendChild(item);
                });
                document.querySelector(".profile-container").appendChild(purchasesContainer);
            }
        })
        .catch(error => console.error("Ошибка загрузки покупок:", error));

        document.getElementById("topup-btn").addEventListener("click", function (event) {
            event.preventDefault();
            const amount = parseInt(document.getElementById("topup-amount").value);
            if (!amount || amount <= 0) {
                alert("Пожалуйста, введите корректную сумму");
                return;
            }
            fetch('/add_balance', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({ action: "add_balance", login: user.login, balance: amount })
            })
            .then(response => response.json())
            .then(data => {
                user.balance = data.balance;
                localStorage.setItem('user', JSON.stringify(user));
                document.getElementById("balance").textContent = `${data.balance} руб`;
                document.getElementById("topup-amount").value = '';
                alert('Баланс успешно пополнен!');
            })
            .catch(error => {
                console.error('Ошибка:', error);
                alert('Произошла ошибка при пополнении баланса');
            });
        });

        document.getElementById("logout-btn").addEventListener("click", function (event) {
            event.preventDefault();
            localStorage.removeItem("user");
            window.location.href = "/";
        });
    } else {
        window.location.href = "login";
    }
});
