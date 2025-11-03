# 🚀 JS vs. WASM: Performance Showdown

A performance comparison and experimental playground pitting **native JavaScript** against **WebAssembly (Wasm)** for image processing tasks. Discover the overhead and the ultimate efficiency gains of Wasm!

---

## 💡 The Experiment: Histogram Equalization

This repository demonstrates the execution speed of a **Histogram Equalization** algorithm implemented in two ways:

1.  **Pure JavaScript (JS):** Running directly in the browser's main thread.
2.  **WebAssembly (Wasm):** C++ code compiled with Emscripten, executing near-native speed.

Vibe coded with gemini :D


---

## 🛠️ Build and Setup

### Prerequisites

You must have **Node.js** and **Emscripten** installed globally on your system to build the Wasm module.

### Building the WebAssembly Module

The Wasm module is compiled from a C++ source file located in the `cpp` directory.

To build the optimized `.wasm` file, run the following script:

```bash
./build.sh
```

### Running the Project

To launch the local development server and view the experiment in your browser, use the standard npm command:

```bash
npm install
npm run start
```

# 💻 Usage
Open the application in your browser (http://localhost:3000 or similar).

Drag and drop an image onto the designated drop zone.

Click the button to apply the Histogram Equalization filter.

The performance metrics will be printed to your browser's Developer Console.

# 📊 Key Results
The performance analysis yields compelling insights regarding the trade-offs:

Small Images: For smaller input images, the necessary overhead of transferring image data to and from the Wasm memory space often makes the performance gain negligible, or even results in slower execution compared to native JavaScript.

Large Images: This is where WebAssembly shines! Once the computational load outweighs the data transfer overhead, Wasm's near-native execution speed demonstrates clear superiority.

On a large 32 Megapixel image, the WebAssembly implementation was over 2 times faster than its pure JavaScript counterpart.