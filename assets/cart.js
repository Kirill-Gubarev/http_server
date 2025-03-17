document.addEventListener("DOMContentLoaded", async function () {
    function updateHead() {
        const user = JSON.parse(localStorage.getItem("user"));
        if (user) {
            document.getElementById("balance").innerHTML = `<i class="fas fa-wallet"></i> Баланс: ${user.balance} руб`;
            document.getElementById("login").innerHTML = `<i class="fas fa-user"></i> ${user.login}</a>`;
        }
    }
    const cartContainer = document.getElementById("cart-container");

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
            updateHead();  // Обновляем баланс после загрузки корзины
        } catch (error) {
            console.error("Ошибка запроса корзины:", error);
            cartContainer.innerHTML = "<p>Не удалось загрузить корзину.</p>";
        }
    }


    function renderCart(items) {
        cartContainer.innerHTML = "";

        if (items.length === 0) {
            cartContainer.innerHTML = "<p>Ваша корзина пуста.</p>";
            return;
        }

        items.forEach(item => {
            const itemElement = document.createElement("div");
            itemElement.classList.add("product");

            itemElement.innerHTML = `
                <img src="${item.image}" alt="${item.name}">
                <p>${item.name} - ${item.price} руб.</p>
                <p>В наличии: ${item.product_quantity} шт.</p>
                <div class="cart-controls">
                    <label for="quantity-${item.name}">Количество:</label>
                    <input type="number" id="quantity-${item.name}" value="${item.quantity}" min="1" max="${item.product_quantity}">
                    <button class="remove-cart-btn" data-name="${item.name}">Удалить</button>
                </div>
            `;

            const quantityInput = itemElement.querySelector(`#quantity-${item.name}`);
            
            // Добавляем обработчик события для ввода значения
            quantityInput.addEventListener("input", function(event) {
                const max = parseInt(quantityInput.getAttribute("max"), 10);
                let value = quantityInput.value;

                // Удаляем все символы, которые не являются цифрами
                value = value.replace(/[^0-9]/g, '');

                // Обновляем значение в поле
                quantityInput.value = value;

                // Ограничиваем ввод значением max
                if (parseInt(value, 10) > max) {
                    quantityInput.value = max;
                }
            });

            cartContainer.appendChild(itemElement);
        });

        document.querySelectorAll(".remove-cart-btn").forEach(button => {
            button.addEventListener("click", removeCartItem);
        });
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
            await fetchCart();  // Перезагружаем корзину
            updateHead();    // Обновляем баланс после удаления товара
        } catch (error) {
            console.error("Ошибка удаления товара:", error);
        }
    }

    await fetchCart();
});
