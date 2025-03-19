document.addEventListener("DOMContentLoaded", function () {
    const userMenu = document.getElementById("user-menu");

    
    const user = JSON.parse(localStorage.getItem("user"));

    if (user) {
        
        userMenu.innerHTML = `
            <span id="balance">
                <i class="fas fa-wallet"></i> Баланс: ${user.balance} руб
            </span>
            <a href="cart" style="margin-right: 10px;"><i class="fas fa-shopping-cart"></i> Корзина</a>
            <a href="profile"><i class="fas fa-user"></i> ${user.login}</a>
        `;
    }
});

async function fetchProducts() {
    try {
        const response = await fetch(`fruits`);
        const products = await response.json();

        localStorage.setItem('products', JSON.stringify(products));

        const productList = document.getElementById('product-list');
        productList.innerHTML = ''; 

        
        products.sort((a, b) => a.name.localeCompare(b.name));

        products.forEach(product => {
            const productElement = document.createElement('div');
            productElement.classList.add('product');

            const productHTML = `
                <img src="${product.image}" alt="${product.name}">
                <p>${product.name} - ${product.price} руб/кг</p>
                <p>В наличии: ${product.quantity} кг</p> 
                <button class="add-to-cart-btn" data-id="${product.id}" data-name="${product.name}" data-price="${product.price}" 
                        data-image="${product.image}" data-quantity="${product.quantity}">
                    В корзину
                </button>
            `;
            productElement.innerHTML = productHTML;
            productList.appendChild(productElement);
        });

        
        if(localStorage.getItem("user")){
            document.querySelectorAll('.add-to-cart-btn').forEach(button => {
                button.addEventListener('click', openCartModal);
            });
        }
        else{
            document.querySelectorAll('.add-to-cart-btn').forEach(button => {
                button.addEventListener('click', function() {
                    window.location.href = "login";
                });
            });
        }
    } catch (error) {
        console.error('Ошибка при загрузке товаров:', error);
    }
}

function openCartModal(event) {
    const product = event.target;
    const productId = product.getAttribute('data-id');
    const productName = product.getAttribute('data-name');
    const productPrice = product.getAttribute('data-price');
    const productImage = product.getAttribute('data-image');
    const productquantity = parseInt(product.getAttribute('data-quantity'), 10); 

    
    document.getElementById('product-name').textContent = productName;
    document.getElementById('product-price').textContent = productPrice;
    document.getElementById('product-image').src = productImage;
    document.getElementById('product-quantity').textContent = productquantity;

    
    const quantityInput = document.getElementById('quantity');
    quantityInput.value = 1; 
    quantityInput.max = productquantity; 

    
    quantityInput.oninput = function () {
        
        let value = this.value.replace(/[^0-9]/g, '');

        
        if (parseInt(value, 10) > productquantity) {
            value = productquantity;
        }

        
        this.value = value;
    };


    
    document.getElementById('cart-modal').style.display = 'block';

    
    document.getElementById('add-to-cart').onclick = function() {
        const quantity = parseInt(quantityInput.value, 10);
        addToCart(productId, productName, productPrice, quantity);
        closeModal();
    };

    
    document.getElementById('cancel').onclick = closeModal;
    document.getElementById('close-modal').onclick = closeModal;
}


function closeModal() {
    document.getElementById('cart-modal').style.display = 'none';
}

function addToCart(productId, productName, productPrice, quantity) {
    let cart = JSON.parse(localStorage.getItem('cart')) || [];
    const product = {
        id: productId,
        name: productName,
        price: productPrice,
        quantity: quantity
    };
    cart.push(product);
    localStorage.setItem('cart', JSON.stringify(cart));

    
    sendCartData(productName, quantity);
}


async function sendCartData(productName, quantity) {
    
    const user = JSON.parse(localStorage.getItem('user'));

    if (!user) {
        console.error('Пользователь не найден');
        return;
    }

    const data = {
        action: 'add_cart',
        login: user.login,
        product: productName,
        quantity: quantity
    };

    fetch("cart", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(data),  
    })
    .catch(error => {
        console.error("Ошибка:", error);
        alert("Произошла ошибка при подключении к серверу.");
    });
}

document.addEventListener('DOMContentLoaded', fetchProducts);
