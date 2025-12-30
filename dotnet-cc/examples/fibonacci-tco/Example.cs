using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

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

        private static unsafe extern void hostFuncPrintk(uint a);

        private static unsafe extern int hostFuncLenInputBuf();

        private static unsafe extern int hostFuncReadValue(int i);

        private static unsafe extern void hostFuncShutdown();

        [UnmanagedCallersOnly(EntryPoint = "Example_hostFuncPrintkWrapper")]
        private static void hostFuncPrintkWrapper(uint a) {
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
        private static delegate* unmanaged<uint, void> printk = &hostFuncPrintkWrapper;
        private static delegate* unmanaged<void> shutdown = &hostFuncShutdownWrapper;

        public uint fibonacci(uint n, uint a, uint b) {
            if (n == 0) {
                return a;
            }
            return fibonacci(n-1, b, a+b);
        }

        [UnmanagedCallersOnly(EntryPoint = "Example_Run")]
        public static void Run()
        {
            var example = new Example();

            uint y = example.fibonacci(40, 0, 1);
            printk(y);
        }
    }
}

