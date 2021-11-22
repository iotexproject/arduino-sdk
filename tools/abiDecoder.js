const abiDecoder = require('abi-decoder'); // NodeJS
const { decode } = require('punycode');
module.paths.push('/usr/local/lib/node_modules/');
const Web3 = require("web3")
var web3 = new Web3(Web3.givenProvider || "ws://localhost:8546");   // Add anything here, we don't use it but it's needed for the library


// --------------------- ENCODE ------------------------------

var functionAbi = {
    "inputs": [
        {
            "name": "input1",
            "type": "uint8[]"
        }
    ],
    "name": "functionName",
    "outputs": [],
    "stateMutability": "nonpayable",
    "type": "function"
};

var params = [
    [1,2]
];

var encodedData = web3.eth.abi.encodeFunctionCall(
   functionAbi, params);

console.log("Encoded data:\n" + encodedData + "\n")





// --------------------- DECODE ------------------------------

// Erc20 ABI
// const testABI = [{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[{"name":"","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"_owner","type":"address"},{"name":"_spender","type":"address"}],"name":"allowance","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"payable":true,"stateMutability":"payable","type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"owner","type":"address"},{"indexed":true,"name":"spender","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Approval","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Transfer","type":"event"}];

// Use custom function abi
const testABI = [functionAbi];

abiDecoder.addABI(testABI);

testData = encodedData;
// testData = "0x000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000e000000000000000000000000000000000000000000000000000000000000000036f6e650000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000374776f000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000057468726565000000000000000000000000000000000000000000000000000000";


decodedData = abiDecoder.decodeMethod(testData);
console.log("Decoded data: \n")
console.log("Function: " + decodedData.name)

let paramCounter = 0
decodedData.params.forEach(param => {
    console.log("Param " + paramCounter++ + ": ")
    console.log("Name: " + param.name)
    console.log("Type: " + param.type)
    console.log("Value: " + param.value)
});

