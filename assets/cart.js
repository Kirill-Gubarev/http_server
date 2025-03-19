document.addEventListener("DOMContentLoaded", async function () {
    function updateHead() {
        const user = JSON.parse(localStorage.getItem("user"));
        if (user) {
            document.getElementById("balance").innerHTML = `<i class="fas fa-wallet"></i> Баланс: ${user.balance} руб`;
            document.getElementById("login").innerHTML = `<i class="fas fa-user"></i> ${user.login}</a>`;
        }
    }

    const cartContainer = document.getElementById("cart-container");
    const purchaseBtnContainer = document.getElementById("purchase-btn");

    async function fetchCart() {
        const user = JSON.parse(localStorage.getItem("user"));
        try {
            const response = await fetch("cart", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ action: "get_cart", login: user.login })
            });

            if (!response.ok) throw new Error("Ошибка загрузки корзины");
            const cartItems = await response.json();
            renderCart(cartItems);
            updateHead();
        } catch (error) {
            console.error("Ошибка запроса корзины:", error);
            cartContainer.innerHTML = "<p>Не удалось загрузить корзину.</p>";
        }
    }

    function renderCart(items) {
        cartContainer.innerHTML = "";
        purchaseBtnContainer.innerHTML = ""; 

        if (items.length === 0) {
            cartContainer.innerHTML = "<p>Ваша корзина пуста.</p>";
            return; 
        }

        items.forEach((item, index) => {
            const itemElement = document.createElement("div");
            itemElement.classList.add("product");

            const inputId = `quantity-${index}-${item.name.replace(/\s+/g, '-')}`;

            itemElement.innerHTML = `
                <img src="${item.image}" alt="${item.name}">
                <p>${item.name} - ${item.price} руб.</p>
                <p>В наличии: ${item.product_quantity} кг.</p>
                <div class="cart-controls">
                    <label for="${inputId}">Количество:</label>
                    <input type="number" id="${inputId}" value="${item.quantity}" min="1" max="${item.product_quantity}">
                    <button class="remove-cart-btn" data-name="${item.name}">Удалить</button>
                </div>
            `;

            cartContainer.appendChild(itemElement);
            const quantityInput = itemElement.querySelector(`#${inputId}`);

            if (quantityInput) {
                quantityInput.addEventListener("input", function(event) {
                    const max = parseInt(quantityInput.getAttribute("max"), 10);
                    let value = quantityInput.value;

                    value = value.replace(/[^0-9]/g, '');
                    quantityInput.value = value;

                    if (parseInt(value, 10) > max) {
                        quantityInput.value = max;
                    }
                    updateTotalPrice();
                });
            } else {
                console.error(`Элемент с id ${inputId} не найден`);
            }
        });

        document.querySelectorAll(".remove-cart-btn").forEach(button => {
            button.addEventListener("click", removeCartItem);
        });

        
        const totalContainer = document.createElement("div");
        totalContainer.classList.add("total-price");

        const purchaseButton = document.createElement("button");
        purchaseButton.textContent = "Купить";
        purchaseButton.classList.add("purchase-btn");
        purchaseButton.addEventListener("click", purchaseCart);

        purchaseBtnContainer.appendChild(totalContainer);
        purchaseBtnContainer.appendChild(purchaseButton);

        function updateTotalPrice() {
            let total = 0;
            const products = cartContainer.querySelectorAll(".product");

            products.forEach(product => {
                const priceText = product.querySelector("p").textContent;
                const price = parseFloat(priceText.split(" - ")[1]);
                const quantity = parseInt(product.querySelector("input").value);
                total += price * quantity;
            });

            totalContainer.textContent = `Общая сумма: ${total.toFixed(2)} руб.`;
        }

        updateTotalPrice();
    }

    async function removeCartItem(event) {
        const itemName = event.target.dataset.name;
        const user = JSON.parse(localStorage.getItem("user"));

        try {
            const response = await fetch("cart", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ action: "remove_cart", login: user.login, product: itemName })
            });

            if (!response.ok) throw new Error("Ошибка удаления товара");
            await fetchCart();
            updateHead();
        } catch (error) {
            console.error("Ошибка удаления товара:", error);
        }
    }

    async function purchaseCart() {
        const user = JSON.parse(localStorage.getItem("user"));
        const cartItems = document.querySelectorAll(".product");

        if (!user || cartItems.length === 0) {
            alert("Корзина пуста или пользователь не найден!");
            return;
        }

        const purchaseList = [];
        let totalPrice = 0;

        
        cartItems.forEach(item => {
            const productName = item.querySelector("p").textContent.split(" - ")[0];
            const quantityInput = item.querySelector("input");
            const quantity = parseInt(quantityInput.value, 10);
            const priceText = item.querySelector("p").textContent;
            const price = parseFloat(priceText.split(" - ")[1]);

            totalPrice += price * quantity;

            purchaseList.push({
                product: productName,
                quantity: quantity,
                price: price
            });
        });

        
        if (totalPrice > user.balance) {
            alert("Недостаточно средств на балансе для оформления покупки.");
            return;
        }

        try {
            const response = await fetch("purchase", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({
                    action: "add_purchase",
                    login: user.login,
                    items: purchaseList
                })
            });

            const result = await response.json();

            switch (result.success) {
                case 1:
                    await updateUserBalance(user.login);
                    alert("Покупка успешно завершена.");
                    cartContainer.innerHTML = "<p>Покупка завершена. Корзина очищена.</p>";
                    purchaseBtnContainer.innerHTML = ""; 
                    break;
                case 2:
                    alert("Недостаточно товара на складе.");
                    await fetchCart();
                    break;
                case 3:
                    alert("Недостаточно средств на балансе.");
                    await fetchCart();
                    break;
                case 0:
                default:
                    alert("Ошибка на сервере. Попробуйте позже.");
                    break;
            }
        } catch (error) {
            console.error("Ошибка покупки:", error);
            alert("Ошибка при обработке покупки.");
        }
    }


    async function updateUserBalance(login) {
        try {
            const response = await fetch("user", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ action: "get_user", login: login })
            });

            if (!response.ok) throw new Error("Ошибка запроса данных пользователя");

            const userData = await response.json();

            if (userData.success) {
                const updatedUser = {
                    ...JSON.parse(localStorage.getItem("user")),
                    balance: userData.balance
                };
                localStorage.setItem("user", JSON.stringify(updatedUser));
                updateHead();
            } else {
                alert("Ошибка получения данных пользователя.");
            }
        } catch (error) {
            console.error("Ошибка при обновлении данных пользователя:", error);
            alert("Ошибка при обновлении данных пользователя.");
        }
    }

    await fetchCart();
});
