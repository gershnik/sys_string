

WebAssembly.instantiate = function() {
    let originalInstantiate = WebAssembly.instantiate;
    return function(binary, imports) {
        return originalInstantiate(binary, imports, {
            builtins: ["js-string"]
        });
    }
}();




