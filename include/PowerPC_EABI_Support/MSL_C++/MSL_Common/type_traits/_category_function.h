//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

template <typename T>
struct is_function : public false_type {};

template <typename Ret>
struct is_function<Ret (*)()> : public true_type {};

template <typename Ret>
struct is_function<Ret (*)(...)> : public true_type {};

template <typename Ret, typename T1>
struct is_function<Ret (*)(T1)> : public true_type {};

template <typename Ret, typename T1>
struct is_function<Ret (*)(T1, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2>
struct is_function<Ret (*)(T1, T2)> : public true_type {};

template <typename Ret, typename T1, typename T2>
struct is_function<Ret (*)(T1, T2, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3>
struct is_function<Ret (*)(T1, T2, T3)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3>
struct is_function<Ret (*)(T1, T2, T3, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4>
struct is_function<Ret (*)(T1, T2, T3, T4)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4>
struct is_function<Ret (*)(T1, T2, T3, T4, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5>
struct is_function<Ret (*)(T1, T2, T3, T4, T5)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, ...)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16)> : public true_type {};

template <typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16>
struct is_function<Ret (*)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, ...)> : public true_type {};