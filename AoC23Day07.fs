open System.IO

let ParseData (s:string) =
    let split = s.Split(' ')
    split[0], System.Int64.Parse split[1]

let Histogram hand =
    Array.fold(fun (h: int64 array) (v: int64) -> h[(int)v] <- h[(int)v]+1L
                                                  h) (Array.create 13 0L) hand
    
let HandValue1 hand =
    let values = Array.map(fun c -> match c with | 'A' -> 12L
                                                 | 'K' -> 11L
                                                 | 'Q' -> 10L
                                                 | 'J' -> 9L
                                                 | 'T' -> 8L
                                                 | _ -> (int64)(c - '2')) hand
    let hist = Histogram values |> Array.sortDescending
    Array.fold(fun t x -> t * 13L + x) (hist[0] * 5L  + hist[1]) values

let HandValue2 hand =
    let values = Array.map(fun c -> match c with | 'A' -> 12L
                                                 | 'K' -> 11L
                                                 | 'Q' -> 10L
                                                 | 'T' -> 9L
                                                 | 'J' -> 0L
                                                 | _ -> (int64)(c - '1')) hand
    let hist = Histogram values
    let noJokers = Array.sortDescending hist[1..]
    Array.fold(fun t x -> t * 13L + x) ((noJokers[0]+hist[0]) * 5L + noJokers[1]) values

let GetWinnings data ranker = 
    Array.map(fun ((s:string), bet) -> (ranker (s.ToCharArray()), bet)) data |> Array.sortBy fst |> Array.map snd |> Array.indexed |> Array.fold(fun t (i, b) -> t + ((int64)(i+1) * b)) 0L

[<EntryPoint>]
let main argv =
    let data = File.ReadAllLines(argv[0]) |> Array.map ParseData
    printfn "Part 1: %A" <| GetWinnings data HandValue1
    printfn "Part 2: %A" <| GetWinnings data HandValue2
    0