using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;

using Org.BouncyCastle.Crypto.Digests;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;

namespace Example
{
    public unsafe class Example
    {
        [UnmanagedCallersOnly(EntryPoint = "Example_Free")]
        public static void Free(void* p)
        {
            NativeMemory.Free(p);
        }

        // Host functions need to be wrapped within Unmanaged Code
        // as this is the code which can be patched on a wasm level.
        //
        // The managed code on the other hand is compiled to dotnet
        // byte code, and embedded as a binary blob.

        private static unsafe extern void hostFuncPrintk(int a);

        private static unsafe extern int hostFuncLenInputBuf();

        private static unsafe extern int hostFuncReadValue(int i);

        private static unsafe extern void hostFuncShutdown();

        [UnmanagedCallersOnly(EntryPoint = "Example_hostFuncPrintkWrapper")]
        private static void hostFuncPrintkWrapper(int a) {
            hostFuncPrintk(a);
        }

        [UnmanagedCallersOnly(EntryPoint = "Example_hostFuncLenInputBufWrapper")]
        private static int hostFuncLenInputBufWrapper() {
            return hostFuncLenInputBuf();
        }

        [UnmanagedCallersOnly(EntryPoint = "Example_hostFuncReadValueWrapper")]
        private static int hostFuncReadValueWrapper(int i) {
            return hostFuncReadValue(i);
        }

        [UnmanagedCallersOnly(EntryPoint = "Example_hostFuncShutdownWrapper")]
        private static void hostFuncShutdownWrapper() {
            hostFuncShutdown();
        }

        private static delegate* unmanaged<int> lenInputBuf = &hostFuncLenInputBufWrapper;
        private static delegate* unmanaged<int, int> readValue = &hostFuncReadValueWrapper;
        private static delegate* unmanaged<int, void> printk = &hostFuncPrintkWrapper;
        private static delegate* unmanaged<void> shutdown = &hostFuncShutdownWrapper;

        private static byte[] inputData() {
            int n = lenInputBuf();
            byte[] result = new byte[n];

            for (int i = 0; i < n; i++) {
                result[i] = (byte) readValue(i);
            }

            return result;
        }

        public void asciiImage() {
            Console.WriteLine("asciiImage: read image...");
            var newWidth = 80;
            using var image = Image.Load<Rgba32>(inputData());

            int newHeight = (int)(image.Height / (double)image.Width * newWidth * 0.5);
            image.Mutate(x => x.Resize(newWidth, newHeight));

            string chars = "@%#*+=-:. ";

            for (int y = 0; y < image.Height; y++)
            {
                for (int x = 0; x < image.Width; x++)
                {
                    var pixel = image[x, y];
                    int gray = (int)(0.2126 * pixel.R + 0.7152 * pixel.G + 0.0722 * pixel.B);
                    int index = (gray * (chars.Length - 1)) / 255;
                    Console.Write(chars[index]);
                }
                Console.Write('\n');
            }
        }

        static void HashTest()
        {
            var input = inputData();
            Sha256Digest digest = new Sha256Digest(); // 256-bit output
            digest.BlockUpdate(input, 0, input.Length);
            byte[] result = new byte[digest.GetDigestSize()];
            digest.DoFinal(result, 0);

            string hashHex = BitConverter.ToString(result).Replace("-", "").ToLowerInvariant();
            Console.WriteLine($"sha256 of input data:: {hashHex}");
        }

        public void memoryTest() {
            // 400 MB allocated through wasmAllocate
            // 512 MB in principle available from ziskemu
            Console.WriteLine("test filling up memory...\n");

            int memChunkSz = 1024*1024;
            int numChunks = 400;
            byte[][] memChunks = new byte[numChunks][];
            var j = 0;
            for (var i = 1; i <= numChunks; i++)
            {
                memChunks[i-1] = new byte[memChunkSz];
                if (i % 10 == 0)
                {
                    Console.WriteLine($"mem test: filled up {i} MB...");
                }
            }
        }

        [UnmanagedCallersOnly(EntryPoint = "Example_Run")]
        public static void Run()
        {
            var example = new Example();
            example.memoryTest();
            GC.Collect();
            example.memoryTest();
            GC.Collect();

            HashTest();

            bool isBmp = inputData().Length >= 2 && inputData()[0] == 0x42 && inputData()[1] == 0x4D;
            if (isBmp)
            {
                Console.WriteLine("bmp image detected:");
                example.asciiImage();
            }
        }
    }
}

